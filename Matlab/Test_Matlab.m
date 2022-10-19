clc
clear all
close all
tic
%img=imread('E:\Projects\uni-karshenasi\PROJECT\Documents\4_Our Code\Matlab\Test.jpg');
v = VideoReader('Test.mp4');
counter=0;
while hasFrame(v)
counter=counter+1;
img = readFrame(v);
TRL=1;
TRH=1;
TCL=1;
TCH=1;
for irow=1:1:720
    for icol=1:1:1280
        R=img(irow,icol,1);
        G=img(irow,icol,2);
        B=img(irow,icol,3);
        X=G-B/2-R/2;
        if X>45
            if G>=100 && G<=255 && B>=0 && B<=95 && R>=0 && R<=143
            img(irow,icol,1)=0;
            img(irow,icol,2)=0;
            img(irow,icol,3)=0;
            end
        end
    end
end
%imshow(img);
%imwrite(img,'img.jpg')
if counter==200
    break;
end
end
toc