package com.jesson.videocompression;

public class VideoCompression {
    static {
        System.loadLibrary("native-lib");
    }


    public native void compressVideo(String[] compressCommand,CompressionCallback compressionCallback);

    public interface CompressionCallback{
        void onCompress(int current,int total);
    }

}
