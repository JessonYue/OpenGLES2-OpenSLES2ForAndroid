# CameraView

### 项目说明

这个项目基于Google的非正式开源的[CameraView](https://github.com/google/cameraview)组件，兼容Camera 1 API和Camera 2 API，提供了相机预览、切换前后摄像头、切换闪光灯、切换预览图片的比例以及拍照等功能。
CameraView这个项目可以帮助Android开发者快速创建一个可以适配不同Android系统和不同Android设备，并且包含各种基本功能的相机界面，但是它是非正式的，所以仍然存在一些兼容性问题和bug，而且原开发者并没有试图再去修复了，这正创建是自己的CameraView项目的原因。



*This is a preview release. The API is subject to change.*

This is not an official Google product.

CameraView aims to help Android developers easily integrate Camera features.

Requires API Level 9. The library uses Camera 1 API on API Level 9-20 and Camera2 on 21 and above.

| API Level | Camera API | Preview View |
|:---------:|------------|--------------|
| 9-13      | Camera1    | SurfaceView  |
| 14-20     | Camera1    | TextureView  |
| 21-23     | Camera2    | TextureView  |
| 24        | Camera2    | TextureView  |

## Features

- Camera preview by placing it in a layout XML (and calling the start method)
- Configuration by attributes
  - Aspect ratio (app:aspectRatio)
  - Auto-focus (app:autoFocus)
  - Flash (app:flash)

## Usage

```xml
<com.google.android.cameraview.CameraView
    android:id="@+id/camera"
    android:layout_width="match_parent"
    android:layout_height="wrap_content"
    android:keepScreenOn="true"
    android:adjustViewBounds="true"
    app:autoFocus="true"
    app:aspectRatio="4:3"
    app:facing="back"
    app:flash="auto"/>
```

```java
    @Override
    protected void onResume() {
        super.onResume();
        mCameraView.start();
    }

    @Override
    protected void onPause() {
        mCameraView.stop();
        super.onPause();
    }
```

You can see a complete usage in the demo app.

## Contribution

See [CONTRIBUTING.md](/CONTRIBUTING.md).
