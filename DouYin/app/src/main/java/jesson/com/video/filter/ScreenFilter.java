package jesson.com.video.widget;

import android.content.Context;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import jesson.com.video.utils.OpenUtils;
import lyq.com.video.R;


public class ScreenFilter {


    private  FloatBuffer  mTextureBuffer;
    private  FloatBuffer mVertextBuffer;
    private  int vTexture;
    private  int vCoord;
    private  int vMatrix;
    private int vPosition;
    private int mProgram;
    private int mWidth;
    private int mHeight;

    public ScreenFilter(Context context) {

        //把camera_vertext内容读出来
        String vertexSource= OpenUtils.readRawTextFile(context, R.raw.camera_vertex);
        String fragSource = OpenUtils.readRawTextFile(context, R.raw.camera_frag);


        //通过字符串创建着色器程序
        //使用opengl
        //一.顶点着色器
        //1.1创建顶点着色器
        int vSharderId = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
        //1.2绑定代码到着色器中
        GLES20.glShaderSource(vSharderId,vertexSource);
        //1.3编译着色器
        GLES20.glCompileShader(vSharderId);
        //1.4主动获取成功失败
        int[] status=new int[1];
        GLES20.glGetShaderiv(vSharderId,GLES20.GL_COMPILE_STATUS,status,0);
        if (status[0] != GLES20.GL_TRUE){
            throw new IllegalStateException("ScreenFitler 顶点着色器配置失败！");
        }

        //二.创建片元着色器
        int fShaderId = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
        //绑定代码到着色器中
        GLES20.glShaderSource(fShaderId,fragSource);
        //编译
        GLES20.glCompileShader(fShaderId);
        GLES20.glGetShaderiv(fShaderId,GLES20.GL_COMPILE_STATUS,status,0);
        if (status[0] != GLES20.GL_TRUE){
            throw new IllegalStateException("ScreenFilter 片元着色器配置失败");
        }


        //三、把着色器塞到程序里面去，GPU
        mProgram = GLES20.glCreateProgram();
        GLES20.glAttachShader(mProgram,vSharderId);
        GLES20.glAttachShader(mProgram,fShaderId);

        //连接着色器
        GLES20.glLinkProgram(mProgram);

        //获取程序是否配置成功
        GLES20.glGetProgramiv(mProgram,GLES20.GL_LINK_STATUS,status,0);
        if (status[0] != GLES20.GL_TRUE){
            throw  new IllegalStateException("Screen Filter 着色器程序连接失败");
        }

        //因为着色器已经塞到了GPU程序中，所以可以删除了
        GLES20.glDeleteShader(vSharderId);
        GLES20.glDeleteShader(fShaderId);

        //获得着色器程序中的变量的索引，通过这个索引对其进行赋值
        vPosition = GLES20.glGetAttribLocation(mProgram,"vPosition");
        vCoord = GLES20.glGetAttribLocation(mProgram,"vCoord");
        vMatrix = GLES20.glGetUniformLocation(mProgram,"vMatrix");

        vTexture =  GLES20.glGetUniformLocation(mProgram,"vTexture");


        //创建一个数据缓冲区
        //OpenGL的中顶点的位置坐标
        mVertextBuffer = ByteBuffer.allocateDirect(4 * 2 * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        mVertextBuffer.clear();
        float[] v = {-1.0f,-1.0f,
                1.0f,-1.0f,
                -1.0f,1.0f,
                1.0f,1.0f
        };

        mVertextBuffer.put(v);

        //采样器采样图片的坐标
        mTextureBuffer = ByteBuffer.allocateDirect(4 * 2 * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        mTextureBuffer.clear();

//        float[] f={0.0f,1.0f,
//                1.0f,1.0f,
//                0.0f,0.0f,
//                1.0f,0.0f
//        };
//
        //顺时针旋转90度
//        float[] f={1.0f,1.0f,
//                1,0,
//                0,1,
//                0,0
//
//
//        };

//        镜像
        float[] f={1.0f,0.0f,
                1.0f,1.0f,
                0.0f,0.0f,
                0.0f,1.0f
        };
        mTextureBuffer.put(f);
    }


    /**
     * 使用着色器程序开始画画
     * @param texture
     * @param mtx
     */
    public void onDrawFrame(int texture,float[] mtx){
        //1.设置窗口大小
        GLES20.glViewport(0,0,mWidth,mHeight);
        //2.使用着色器程序
        GLES20.glUseProgram(mProgram);
        /**
         * 3. 画顶点
         */
        //3.1传入顶点数据，确定形状
        mVertextBuffer.position(0);
        //size:2表示xy两个数据
        GLES20.glVertexAttribPointer(vPosition,2,GLES20.GL_FLOAT,false,0,mVertextBuffer);
        //3.2激活
        GLES20.glEnableVertexAttribArray(vPosition);

        //4，片元，纹理
        mTextureBuffer.position(0);
        GLES20.glVertexAttribPointer(vCoord,2,GLES20.GL_FLOAT,false,0,mTextureBuffer);
        GLES20.glEnableVertexAttribArray(vCoord);

        //5.变换矩阵
        GLES20.glUniformMatrix4fv(vMatrix,1,false,mtx,0);

        //片元
        //激活图层
        GLES20.glActiveTexture(GLES20.GL_TEXTURE);
        //图像数据
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,texture);
        //传递参数
        GLES20.glUniform1i(vTexture,0);

        //参数传完了，通知OpenGL画画，从第0个点开始，共4个点
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP,0,4);
    }


    public void onReady(int width,int height){
        mWidth = width;
        mHeight = height;
    }
}
