package idv.coolshou.fileutils;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.DocumentsContract;

public class FileUtils {

    public static String getPathFromUri(Context context, Uri uri) {
        String result = null;

        if ("content".equals(uri.getScheme())) {
            Cursor cursor = context.getContentResolver().query(uri, null, null, null, null);
            if (cursor != null) {
                try {
                    if (cursor.moveToFirst()) {
                        String documentId = DocumentsContract.getDocumentId(uri);
                        String[] split = documentId.split(":");
                        String type = split[0];

                        if ("primary".equalsIgnoreCase(type)) {
                            result = "/storage/emulated/0/" + split[1];
                        }
                    }
                } finally {
                    cursor.close();
                }
            }
        }
        return result;
    }
}
