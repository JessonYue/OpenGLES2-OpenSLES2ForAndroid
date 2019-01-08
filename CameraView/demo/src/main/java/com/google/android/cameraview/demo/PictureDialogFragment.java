/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.google.android.cameraview.demo;

import android.app.Dialog;
import android.net.Uri;
import android.support.v4.app.DialogFragment;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.facebook.drawee.view.SimpleDraweeView;

import java.io.File;

public class PictureDialogFragment extends DialogFragment {

    private static final String ARG_FILE_PATH = "file_path";

    private SimpleDraweeView mImageView;

    public static PictureDialogFragment newInstance(String filePath) {
        PictureDialogFragment fragment = new PictureDialogFragment();
        Bundle args = new Bundle();
        args.putString(ARG_FILE_PATH, filePath);
        fragment.setArguments(args);
        return fragment;
    }

    @NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Bundle args = getArguments();
        String filePath = args.getString(ARG_FILE_PATH);
        Uri uri = Uri.fromFile(new File(filePath));

        View contentView = LayoutInflater.from(getContext()).inflate(R.layout.dialog_picture, null);
        mImageView = (SimpleDraweeView) contentView.findViewById(R.id.image);
        mImageView.setImageURI(uri);

        Dialog dialog = new Dialog(getContext());
        dialog.setCancelable(true);
        dialog.setCanceledOnTouchOutside(true);
        dialog.setContentView(contentView);

        //make dialog fullscreen
        Window window = dialog.getWindow();
        window.getDecorView().setPadding(0, 0, 0, 0);//消除边距
        WindowManager.LayoutParams layoutParams = window.getAttributes();
        layoutParams.width = WindowManager.LayoutParams.MATCH_PARENT;
        layoutParams.height = WindowManager.LayoutParams.MATCH_PARENT;
        window.setAttributes(layoutParams);

        return dialog;
    }

}