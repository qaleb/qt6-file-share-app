package idv.coolshou.fileutils;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.text.TextUtils;

public class FileUtils {

    public static String getPathFromUri(Context context, Uri uri) {
        String filePath = null;

        // DocumentProvider
        if (DocumentsContract.isDocumentUri(context, uri)) {
            final String docId = DocumentsContract.getDocumentId(uri);
            final String[] split = docId.split(":");
            final String type = split[0];

            switch (type) {
                case "primary":
                    filePath = Environment.getExternalStorageDirectory() + "/" + split[1];
                    break;
                case "image":
                case "video":
                case "audio":
                    Uri contentUri = null;
                    switch (type) {
                        case "image":
                            contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                            break;
                        case "video":
                            contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                            break;
                        case "audio":
                            contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                            break;
                    }
                    filePath = getDataColumn(context, contentUri, "_id=?", new String[]{split[1]});
                    break;
                case "downloads":
                    Uri downloadsUri = Uri.withAppendedPath(
                        Uri.parse("content://downloads/public_downloads"), split[1]);
                    filePath = getDataColumn(context, downloadsUri, null, null);
                    break;
                case "raw":
                    filePath = split[1];
                    break;
                default:
                    if (uri.getAuthority().equals("com.android.externalstorage.documents")) {
                        filePath = "/storage/" + type + "/" + split[1];
                    }
                    break;
            }
        } else if ("content".equalsIgnoreCase(uri.getScheme())) {
            // MediaStore and other similar providers
            filePath = getDataColumn(context, uri, null, null);
        } else if ("file".equalsIgnoreCase(uri.getScheme())) {
            // File URI
            filePath = uri.getPath();
        }

        return filePath;
    }

    private static String getDataColumn(Context context, Uri uri, String selection, String[] selectionArgs) {
        Cursor cursor = null;
        final String column = "_data";
        final String[] projection = {column};

        try {
            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs, null);
            if (cursor != null && cursor.moveToFirst()) {
                final int columnIndex = cursor.getColumnIndexOrThrow(column);
                return cursor.getString(columnIndex);
            }
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }
}
