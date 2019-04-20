var video = document.querySelector('video');
var canvas = document.querySelector('canvas');
var ctx = canvas.getContext('2d');
var localMediaStream = null;

var snapshot = function () {
    if (localMediaStream) {
        ctx.drawImage(video, 0, 0);
        document.querySelector('img').src = canvas.toDataURL('image/webp');
    }
};

var sizeCanvas = function () {
    setTimeout(function () {
        canvas.width = video.videoWidth;
        canvas.height = video.videoHeight;
        img.width = video.videoWidth;
        img.height = video.videoHeight;
    }, 100);
};

var btnCapture = document.getElementById('capture');
btnCapture.addEventListener('click', snapshot, false);

navigator.webkitGetUserMedia(
    {video: true},
    function (stream) {
        video.src = window.URL.createObjectURL(stream);
        localMediaStream = stream;
        sizeCanvas();
    },
    function () {
        alert('your browser does not support getUserMedia');
    }
);
