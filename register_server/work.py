#encoding:utf-8
# import the libraries
import os
import face_recognition
import shutil

# 列出图像文件
def getImage(path):
    f_list = os.listdir(path)
    res = []
    for i in f_list:
        if os.path.splitext(i)[1] == '.jpg':
            res.append(i)
        if os.path.splitext(i)[1] == '.png':
            res.append(i)
    return res

def getTest(path):
    f_list = os.listdir(path)
    for i in f_list:
        if i=='test.png':
            return True
    return False
        
        



def writeResult(res):
    with open('/Users/hong/Downloads/res.txt', 'w') as f :
        f.write(str(res))

# 读取数据库中保存的reference
def loadMatchImage(path):
    res = []

    images = getImage(path)
    print(images)
    
    for img in images:
        
        # load image
        image_to_be_matched = face_recognition.load_image_file(os.path.join(path, img))
        # encode image  
        encode = face_recognition.face_encodings(image_to_be_matched)
        # 可能会找不到人脸
        if len(encode)>0:
            image_to_be_matched_encoded = encode[0]
            res.append(image_to_be_matched_encoded)
            print(os.path.join(path, img), "successfully")
        else:
            print(os.path.join(path, img), "failed")
    #print('There are %d reference photos. '%(len(res)))
    return res


# ============= main ===================
path = 'ref'
#images_to_be_matched_encoded = loadMatchImage(path)
monitor_path = '/Users/hong/Downloads'
testimage = '/Users/hong/Downloads/test.png' 

flag = 0
count = 0

while(True):
    # 扫描ref文件夹
    
    # 扫描downloads文件夹，检测网络图片
    image = getTest(monitor_path)   # bool
    
    #while(len(images)!=0):
    while image == True:
        #images_to_be_matched_encoded = loadMatchImage(path)

        reffiles = getImage(path)
        
        #image = images
        current_image = face_recognition.load_image_file(testimage)
        # 对test进行encode
        current_image_encoded = face_recognition.face_encodings(current_image)[0]
        # 找不到人脸
        if len(current_image_encoded) == 0:
            writeResult(0)
            break

        for ref in reffiles:
            ref_image = face_recognition.load_image_file(path + '/' + ref)
            ref_encoded = face_recognition.face_encodings(ref_image)[0]

            result = face_recognition.compare_faces([ref_encoded], current_image_encoded, 0.3)
        #result = face_recognition.compare_faces(
        #[image_to_be_matched_encoded], current_image_encoded)
         # check if it was a match
            print(result[0])
            if result[0] == True:
                print "Matched: " + testimage + ' and ' + ref
                flag = 1
                break
        if flag==0:
            print "No matched: " + testimage
            writeResult(0)
            shutil.move(testimage, 'ref/'+str(count)+'.png')
            count = count + 1
            
        else:
            flag = 0
            # TOFIXED: more robust name 
            writeResult(1)
            os.remove(testimage)
            
            
        
        image = False

    
   