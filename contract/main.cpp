#include <graphenelib/asset.h>
#include <graphenelib/contract.hpp>
#include <graphenelib/contract_asset.hpp>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/system.h>
#include <vector>

using namespace graphene;

class bank : public contract
{
  public:
    bank(uint64_t account_id)
        : contract(account_id)
        , states(_self, _self)
        , voters(_self, _self)
        , records(_self, _self)
    {
    }
    
    // @abi action
    void init() 
    {
        // %TODO 发起合约的人才能调用
        auto it = states.find(0);
        graphene_assert(it==states.end(), "Have inited");
        uint64_t owner = get_trx_sender();
        states.emplace(owner, [&](auto &o) {
            o.id = 0;
            o.candidatesCount = 0;
            o.flag = false;
            o.endflag = false;
        });
    }
    
    // @abi action
    void initcandidate(vector<std::string> &cands) 
    {
        uint64_t owner = get_trx_sender();
        auto it = states.find(0);
        graphene_assert(it!=states.end(), "Haven't inited");
        
        graphene_assert(!it->flag, "Have inited");
        uint64_t len = cands.size();
        //candidatesCount =  0;
        for (char i=0; i<len; i++) {
            addCandidate(cands[i]);
        }
        states.modify(it, owner, [&](auto &o) {o.flag = true;});
    }
    
    // @abi action
    void deposit()
    {
        auto stateit = states.find(0);
        graphene_assert(!stateit->endflag, "Vote has ended");
        
        uint64_t owner = get_trx_sender();
        auto it = voters.find(owner);
        if (it == voters.end()) {
            voters.emplace(owner, [&](auto &o) {
                o.address = owner;
                o.hasVoted = false;
            });
        } 
    }
    
    // @abi action
    void myvote(uint64_t _candidateId) 
    {
        auto it = states.find(0);
        graphene_assert(!it->endflag, "Vote has ended");
        // 是否已经投票

        uint64_t sender = get_trx_sender();
        auto voter_iter = voters.find(sender);

        graphene_assert(voter_iter != voters.end(), "No this voter");
        graphene_assert(voter_iter->hasVoted==false, "You have voted");
        
        // candidate是否有效
        graphene_assert(_candidateId > 0, "The candidate id is invalid");
        graphene_assert(_candidateId <= it->candidatesCount, "The candidate id is greater than candidatesCount");
        
        // 将这个voter的hasVoted设为true
        // 需要使用modify
        voters.modify(voter_iter, sender, [&](auto &o) {o.hasVoted=true;});
        
        // 
        auto record_iter = records.find(_candidateId);
        records.modify(record_iter, sender, [&](auto &o) {o.voteCount++;});
    }
    
    // @abi action
    void endvote()
    {
        //TODO: 权限设置
        uint64_t owner = get_trx_sender();
        auto it = states.find(0);
        states.modify(it, owner, [&](auto &o) {o.endflag = true;});
    }


  private:
    /*
    uint64_t candidatesCount = 0;
    bool flag = false;
    bool endflag = false;
    */
    void addCandidate(std::string _name) {
        uint64_t owner = get_trx_sender();
        auto it = states.find(0);
        //candidatesCount++;
        states.modify(it, owner, [&](auto &o) {o.candidatesCount++;});
        
        
        records.emplace(owner, [&](auto &o) {
                o.id = it->candidatesCount;
                o.name = _name;
                o.voteCount = 0;
            });
    }

    // @abi table state i64
    struct state {
        uint64_t id;
        uint64_t candidatesCount;
        bool flag;
        bool endflag;
        
        uint64_t primary_key() const {return id;}
        
        GRAPHENE_SERIALIZE(state, (id)(candidatesCount)(flag)(endflag))
    };
    typedef graphene::multi_index<N(state), state> state_index;
    
    
    //@abi table candidate i64
    struct candidate {
        uint64_t id;
        std::string name;
        uint64_t voteCount;
        
        uint64_t primary_key() const {return id;}
        
        GRAPHENE_SERIALIZE(candidate, (id)(name)(voteCount))
    };
    typedef graphene::multi_index<N(candidate), candidate> candidates_index;
    
    //@abi table voter i64
    struct voter {
        uint64_t address;
        bool hasVoted;

        uint64_t primary_key() const { return address; }

        GRAPHENE_SERIALIZE(voter, (address)(hasVoted))
    };
    typedef graphene::multi_index<N(voter), voter> voters_index;


    voters_index            voters;
    candidates_index        records;
    state_index            states;
};

GRAPHENE_ABI(bank, (init)(initcandidate)(deposit)(myvote)(endvote))
