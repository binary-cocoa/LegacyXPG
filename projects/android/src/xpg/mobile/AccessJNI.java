package xpg.mobile;

public class AccessJNI
{
     static
     {
         System.loadLibrary("xpgm");
     }

     public static native void onLoad();
     public static native void onResize(int inWidth, int inHeight);
     public static native void onLoop();
}
