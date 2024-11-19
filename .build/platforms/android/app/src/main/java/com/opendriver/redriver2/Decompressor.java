package com.opendriver.redriver2;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import java.io.*;
import java.nio.file.Files;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class Decompressor {
    private InputStream _zipFile;
    private String _location;

    public Decompressor(InputStream zipFile, String location) {
        _zipFile = zipFile;
        _location = location;

        // Ensure the root directory exists at initialization
        _dirChecker("");
    }

    public void unzip() {
        // Buffers for reading and writing
        byte[] buffer = new byte[8192]; // 8 KB buffer

        try (ZipInputStream zin = new ZipInputStream(new BufferedInputStream(_zipFile))) {
            ZipEntry ze;
            while ((ze = zin.getNextEntry()) != null) {
                // Log and show Toast for the current file
                String entryName = ze.getName();
               // Toast.makeText(context, "Unzipping " + entryName, Toast.LENGTH_SHORT).show();
                Log.v("Decompress", "Unzipping " + entryName);

                if (ze.isDirectory()) {
                    // Create directory if it's a directory entry
                    _dirChecker(entryName);
                } else {
                    // Extract file
                    File outFile = new File(_location, entryName);
                    File parentDir = outFile.getParentFile();

                    // Create the parent directories if they don't exist
                    if (parentDir != null && !parentDir.exists()) {
                        parentDir.mkdirs();
                    }

                    // Use try-with-resources to handle file output stream automatically
                    try (BufferedOutputStream fout = new BufferedOutputStream(Files.newOutputStream(outFile.toPath()))) {
                        int bytesRead;
                        while ((bytesRead = zin.read(buffer)) != -1) {
                            fout.write(buffer, 0, bytesRead);
                        }
                    }
                }

                zin.closeEntry(); // Close the current entry
            }
        } catch (IOException e) {
            Log.e("Decompress", "Error while unzipping", e);
        }
    }

    private void _dirChecker(String dir) {
        // Ensure that the directory exists or create it
        File dirFile = new File(_location, dir);
        if (!dirFile.exists()) {
            dirFile.mkdirs();
        }
    }
}
