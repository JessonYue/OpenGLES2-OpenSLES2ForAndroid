package jesson.com.video.widget;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import jesson.com.video.utils.CameraHelper;

public class DouyinRender implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    private jesson.com.video.widget.ScreenFilter mScreenFilter;
    private DouyinView mView;
    private CameraHelper mCameraHelper;
    private SurfaceTexture mSurfaceTexture;
    private int[] mTextures;
    private float[] mtx = new float[16];

    public DouyinRender(DouyinView mView) {
        this.mView = mView;
    }


    /**
     * 画布创建好了
     * @param gl
     * @param config
     */
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        //初始化操作
        mCameraHelper = new CameraHelper(Camera.CameraInfo.CAMERA_FACING_BACK);

        //准备好摄像头绘制的画布
        //通过gl创建一个纹理id
        mTextures = new int[1];
        GLES20.glGenTextures(mTextures.length,mTextures,0);
        mSurfaceTexture=new SurfaceTexture(mTextures[0]);
        mSurfaceTexture.setOnFrameAvailableListener(this);

        //必须要glThread中进行初始化
        mScreenFilter=new jesson.com.video.widget.ScreenFilter(mView.getContext());

    }

    /**
     * 画布改变了
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
     * 开始画画了
     * @param gl
     */
    @Override
    public void onDrawFrame(GL10 gl) {
        //配置屏幕
        //1.清理屏幕
        GLES20.glClearColor(0,0,0,0);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

        //把摄像头的数据先输出来
        //更新纹理
        mSurfaceTexture.updateTexImage();
        //获得变换矩阵
        mSurfaceTexture.getTransformMatrix(mtx);
        mScreenFilter.onDrawFrame(mTextures[0],mtx);

    }

    /**
     * 当有一个可用帧时调用
     * @param surfaceTexture
     */
    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        //因为使用的是按需渲染，所以当有一个可用帧的时候，就去请求渲染一次
        mView.requestRender();
    }
}
