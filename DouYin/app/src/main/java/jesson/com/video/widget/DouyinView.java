package jesson.com.video.widget;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;


public class DouyinView extends GLSurfaceView {


    public DouyinView(Context context) {
        this(context,null);
    }

    public DouyinView(Context context, AttributeSet attrs) {
        super(context, attrs);
        //配置EGL版本
        setEGLContextClientVersion(2);
        //设置渲染器
        setRenderer(new DouyinRender(this));
        //设置渲染模式
        setRenderMode(RENDERMODE_WHEN_DIRTY);

    }
}
