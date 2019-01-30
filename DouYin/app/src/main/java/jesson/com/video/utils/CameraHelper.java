package jesson.com.video.utils;

import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;

import java.io.IOException;

/***
 * Camera帮助类
 */
public class CameraHelper implements Camera.PreviewCallback {

    private static final String TAG = "JessonCameraHelper";
    private static final int WIDTH = 640;
    private static final int HEIGHT = 480;

    private int mCameraId;
    private Camera mCamera;
    private SurfaceTexture mSurfaceTexture;
    private byte[] buffer;
    private Camera.PreviewCallback mPreviewCallback;


    public CameraHelper(int cameraId) {
        mCameraId = cameraId;
    }

    public void switchCamera() {
        if (mCameraId == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            mCameraId = Camera.CameraInfo.CAMERA_FACING_BACK;
        } else {
            mCameraId = Camera.CameraInfo.CAMERA_FACING_FRONT;
        }
        stopPreview();
        startPreview(mSurfaceTexture);
    }


    public void setmPreviewCallback(Camera.PreviewCallback mPreviewCallback) {
        this.mPreviewCallback = mPreviewCallback;
    }

    public int getmCameraId() {
        return mCameraId;
    }

    public void startPreview(SurfaceTexture surfaceTexture) {

        mSurfaceTexture = surfaceTexture;
        try {
            mCamera = Camera.open(mCameraId);
            Camera.Parameters parameters = mCamera.getParameters();
            //设置数据格式
            parameters.setPreviewFormat(ImageFormat.NV21);
            parameters.setPreviewSize(WIDTH, HEIGHT);
            mCamera.setParameters(parameters);

            //设置数据缓存区
            buffer = new byte[WIDTH * HEIGHT * 3];
            mCamera.addCallbackBuffer(buffer);
            mCamera.setPreviewCallback(this);
            //设置预览界面
            mCamera.setPreviewTexture(mSurfaceTexture);
            mCamera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    public void stopPreview() {
        if (mCamera != null) {
            //数据停止回调接口
            mCamera.setPreviewCallback(null);
            //停止预览
            mCamera.stopPreview();
            //释放摄像头
            mCamera.release();
            mCamera = null;
        }

    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        if (null != mPreviewCallback) {
            mPreviewCallback.onPreviewFrame(data, camera);
        }

        camera.addCallbackBuffer(buffer);
    }
}
