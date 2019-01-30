package jesson.com.video.widget;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import jesson.com.video.filter.ScreenFilter;
import jesson.com.video.utils.CameraHelper;


public class DouyinRender implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    private ScreenFilter mScreenFilter;
    private DouyinView mView;
    private CameraHelper mCameraHelper;
    private SurfaceTexture mSurfaceTexture;
    private int[] mTextures;
    private float[] mtx = new float[16]; //4*4 矩阵

    public DouyinRender(DouyinView mView) {
        this.mView = mView;
    }


    /**
     * Surface创建好了
     * @param gl
     * @param config
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        //初始化camerahelper帮助类
        mCameraHelper = new CameraHelper(Camera.CameraInfo.CAMERA_FACING_BACK);

        mTextures = new int[1];
        //创建空间纹理
        GLES20.glGenTextures(mTextures.length,mTextures,0);
        mSurfaceTexture=new SurfaceTexture(mTextures[0]);
        /**
         * Register a callback to be invoked when a new image frame becomes available to the
         *      * SurfaceTexture.
         */
        mSurfaceTexture.setOnFrameAvailableListener(this);
        //注意要在glThread线程中进行初始化
        mScreenFilter=new ScreenFilter(mView.getContext());
    }

    /**
     * Surface changed
     * @param gl
     * @param width
     * @param height
     */
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        //摄像头开启预览
        mCameraHelper.startPreview(mSurfaceTexture);
        //可以去获取摄像头数据了
        mScreenFilter.onReady(width,height);
    }


    /**
     * onDrawFrame
     * @param gl
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        //首先清理屏幕
        GLES20.glClearColor(0,0,0,0);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        //把摄像头的数据先输出来
        //更新纹理
        mSurfaceTexture.updateTexImage();
        //SurfaceTexture获得变换矩阵
        mSurfaceTexture.getTransformMatrix(mtx);
        mScreenFilter.onDrawFrame(mTextures[0],mtx);

    }

    /**
     * 当有一个可用帧时调用，节省资源 可以省电 按照需求进行渲染
     * @param surfaceTexture
     */
    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        //因为使用的是按需渲染，所以当有一个可用帧的时候，就去请求渲染一次
        mView.requestRender();
    }
}
