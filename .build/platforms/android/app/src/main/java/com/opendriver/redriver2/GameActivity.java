package com.opendriver.redriver2;

import android.app.Activity;
import org.libsdl.app.SDLActivity;
import android.provider.Settings;
import android.os.Build;
import android.Manifest;
import android.content.Intent;
import android.widget.Toast;
import android.net.Uri;
import android.os.Bundle;

public class GameActivity extends SDLActivity {

    private static final int PERMISSION_REQUEST_CODE = 1313;

    private void requestExternalStorage() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            try {
                Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                intent.addCategory("android.intent.category.DEFAULT");
                intent.setData(Uri.parse(String.format("package:%s",getApplicationContext().getPackageName())));
                startActivityForResult(intent, PERMISSION_REQUEST_CODE);
            } catch (Exception e) {
                // Intent intent = new Intent();
                // intent.setAction(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                // startActivityForResult(intent, PERMISSION_REQUEST_CODE);
            } 
        } else {
            GameActivity.this.requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE}, PERMISSION_REQUEST_CODE);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                Toast.makeText(this, "Allow permission for storage access!", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public GameActivity() {
        super();
        requestExternalStorage(); 
    }

}
