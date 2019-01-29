package learning.jesson.com.opengl;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class WlGLSurfaceView extends GLSurfaceView {
    public WlGLSurfaceView(Context context) {
        this(context, null);
    }

    public WlGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        setRenderer(new WlRender(context));
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }
}
