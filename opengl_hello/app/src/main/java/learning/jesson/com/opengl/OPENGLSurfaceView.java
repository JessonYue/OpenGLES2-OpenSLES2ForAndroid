package learning.jesson.com.opengl;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class OPENGLSurfaceView extends GLSurfaceView {
    public OPENGLSurfaceView(Context context) {
        super(context);
        init();
    }

    private void init() {
        // 使用opengl2.0版本
        setEGLContextClientVersion(2);
        //存储opengl context
        setPreserveEGLContextOnPause(true);
        // set renderer
        setRenderer(new OPENGLRenderer());
    }

    public OPENGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private class OPENGLRenderer implements Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            GLES20.glClearColor(1.0f, 0.2f, 0.7f, 1.0f);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

        }

        @Override
        public void onDrawFrame(GL10 gl) {
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        }
    }
}
