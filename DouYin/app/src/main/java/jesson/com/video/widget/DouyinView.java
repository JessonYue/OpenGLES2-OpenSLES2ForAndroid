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

        //1.配置EGL版本(必须设置这个)
        setEGLContextClientVersion(2);
        //2.设置渲染器（后面会着重说这个类）
        setRenderer(new DouyinRender(this));
        //设置渲染模式
        /**
         * 有两种模式
         * 1.连续渲染
         * 2.按需渲染
         * 这里选择的是按需渲染
         */
        setRenderMode(RENDERMODE_WHEN_DIRTY);

    }
}
