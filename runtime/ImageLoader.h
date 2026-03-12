#pragma once

#include <string>
#include <functional>
#include <map>
#include <jni.h>

namespace aero {

// Forward declarations
class View;

/**
 * ImageLoader for loading images from URLs.
 * Supports caching and placeholder images.
 * 
 * Usage:
 *   // Load into ImageView
 *   ImageLoader::load("https://example.com/image.jpg", imageView);
 *   
 *   // Load with callback
 *   ImageLoader::load("https://example.com/image.jpg", [](bool success, jobject bitmap) {
 *       if (success) {
 *           // Use bitmap
 *       }
 *   });
 *   
 *   // Load with options
 *   ImageLoader::Options options;
 *   options.placeholder = R.drawable.placeholder;
 *   options.errorImage = R.drawable.error;
 *   options.cacheEnabled = true;
 *   ImageLoader::load("https://example.com/image.jpg", imageView, options);
 */
class ImageLoader {
public:
    /**
     * Options for image loading.
     */
    struct Options {
        int placeholder = 0;      // Resource ID for placeholder
        int errorImage = 0;       // Resource ID for error image
        int width = 0;            // Target width (0 = original)
        int height = 0;           // Target height (0 = original)
        bool cacheEnabled = true; // Use memory cache
        bool diskCacheEnabled = true; // Use disk cache
        bool centerCrop = false;  // Center crop the image
        bool fitCenter = false;   // Fit center the image
        
        Options() = default;
    };
    
    /**
     * Callback for async image loading.
     * @param success Whether the image was loaded successfully
     * @param bitmap The loaded bitmap (null if failed)
     */
    using ImageCallback = std::function<void(bool success, jobject bitmap)>;
    
    /**
     * Load image from URL into an ImageView.
     */
    static void load(const std::string& url, jobject imageView);
    static void load(const std::string& url, jobject imageView, const Options& options);
    
    /**
     * Load image from URL with callback.
     */
    static void load(const std::string& url, ImageCallback callback);
    static void load(const std::string& url, ImageCallback callback, const Options& options);
    
    /**
     * Preload image into cache.
     */
    static void preload(const std::string& url);
    
    /**
     * Cancel loading for a specific ImageView.
     */
    static void cancel(jobject imageView);
    
    /**
     * Clear memory cache.
     */
    static void clearMemoryCache();
    
    /**
     * Clear disk cache.
     */
    static void clearDiskCache();
    
    /**
     * Get cached bitmap (returns nullptr if not cached).
     */
    static jobject getCached(const std::string& url);
    
    /**
     * Initialize with JNI environment (called automatically).
     */
    static void initJNI(JNIEnv* env, jobject context);
    
private:
    static JNIEnv* env;
    static jobject appContext;
    static std::map<std::string, jobject> memoryCache;
    
    static jobject downloadBitmap(const std::string& url);
    static jobject scaleBitmap(jobject bitmap, int width, int height, bool centerCrop);
};

} // namespace aero
