#pragma once

#include <jni.h>
#include <string>
#include <functional>
#include <memory>

namespace aero {

// Forward declaration
class Intent;

/**
 * NotificationChannel - Required for Android 8.0+ (API 26).
 * Groups notifications by category with shared behavior.
 */
class NotificationChannel {
public:
    // Importance levels
    static constexpr int IMPORTANCE_NONE = 0;
    static constexpr int IMPORTANCE_MIN = 1;
    static constexpr int IMPORTANCE_LOW = 2;
    static constexpr int IMPORTANCE_DEFAULT = 3;
    static constexpr int IMPORTANCE_HIGH = 4;
    static constexpr int IMPORTANCE_MAX = 5;
    
    NotificationChannel(const std::string& id, const std::string& name, int importance);
    ~NotificationChannel();
    
    // Configuration (chainable)
    NotificationChannel& setDescription(const std::string& description);
    NotificationChannel& enableVibration(bool enable);
    NotificationChannel& enableLights(bool enable);
    NotificationChannel& setLightColor(int color);
    NotificationChannel& setShowBadge(bool show);
    NotificationChannel& setSound(const std::string& soundUri);
    
    // Build JNI object
    jobject buildJNI(JNIEnv* env) const;
    
    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    
private:
    std::string id;
    std::string name;
    int importance;
    std::string description;
    bool vibrationEnabled;
    bool lightsEnabled;
    int lightColor;
    bool showBadge;
    std::string soundUri;
};

/**
 * NotificationBuilder - Construct notifications with builder pattern.
 * 
 * Usage:
 *   NotificationBuilder builder(env, context, "channel_id");
 *   builder.setTitle("Download Complete")
 *          .setContent("File saved successfully")
 *          .setSmallIcon(R.drawable.ic_download)
 *          .setAutoCancel(true);
 *   
 *   notificationManager.notify(1, builder);
 */
class NotificationBuilder {
public:
    // Priority constants (pre-Android 8.0)
    static constexpr int PRIORITY_MIN = -2;
    static constexpr int PRIORITY_LOW = -1;
    static constexpr int PRIORITY_DEFAULT = 0;
    static constexpr int PRIORITY_HIGH = 1;
    static constexpr int PRIORITY_MAX = 2;
    
    // Visibility constants
    static constexpr int VISIBILITY_PRIVATE = 0;
    static constexpr int VISIBILITY_PUBLIC = 1;
    static constexpr int VISIBILITY_SECRET = -1;
    
    // Default flags
    static constexpr int FLAG_AUTO_CANCEL = 0x00000010;
    static constexpr int FLAG_ONGOING_EVENT = 0x00000002;
    static constexpr int FLAG_NO_CLEAR = 0x00000020;
    
    NotificationBuilder(JNIEnv* env, jobject context, const std::string& channelId);
    ~NotificationBuilder();
    
    // Content
    NotificationBuilder& setTitle(const std::string& title);
    NotificationBuilder& setContent(const std::string& content);
    NotificationBuilder& setSubText(const std::string& subText);
    NotificationBuilder& setTicker(const std::string& ticker);
    
    // Icons
    NotificationBuilder& setSmallIcon(int iconResId);
    NotificationBuilder& setLargeIcon(int iconResId);
    
    // Behavior
    NotificationBuilder& setPriority(int priority);
    NotificationBuilder& setAutoCancel(bool autoCancel);
    NotificationBuilder& setOngoing(bool ongoing);
    NotificationBuilder& setOnlyAlertOnce(bool onlyAlertOnce);
    NotificationBuilder& setVisibility(int visibility);
    
    // Vibration & Sound
    NotificationBuilder& setVibrate(bool vibrate);
    NotificationBuilder& setSound(const std::string& soundUri);
    NotificationBuilder& setSilent(bool silent);
    
    // Progress bar
    NotificationBuilder& setProgress(int max, int progress, bool indeterminate);
    
    // Actions
    NotificationBuilder& setContentIntent(Intent* intent);
    NotificationBuilder& addAction(int iconResId, const std::string& title, Intent* intent);
    NotificationBuilder& setDeleteIntent(Intent* intent);
    
    // Big text style
    NotificationBuilder& setBigText(const std::string& bigText);
    
    // Group notifications
    NotificationBuilder& setGroup(const std::string& groupKey);
    NotificationBuilder& setGroupSummary(bool isSummary);
    
    // Color
    NotificationBuilder& setColor(int color);
    
    // Build the notification JNI object
    jobject build();
    
private:
    JNIEnv* env;
    jobject context;
    jobject builderInstance;
    jclass builderClass;
};

/**
 * NotificationManager - Create channels and show/cancel notifications.
 */
class NotificationManager {
public:
    NotificationManager();
    NotificationManager(JNIEnv* env, jobject context);
    ~NotificationManager();
    
    void initJNI(JNIEnv* env, jobject context);
    
    // Channels (Android 8.0+)
    void createChannel(const NotificationChannel& channel);
    void deleteChannel(const std::string& channelId);
    
    // Show / cancel notifications
    void notify(int notificationId, NotificationBuilder& builder);
    void notify(const std::string& tag, int notificationId, NotificationBuilder& builder);
    void cancel(int notificationId);
    void cancel(const std::string& tag, int notificationId);
    void cancelAll();
    
    // Check if notifications are enabled
    bool areNotificationsEnabled();
    
private:
    JNIEnv* env;
    jobject context;
    jobject managerInstance;
    
    void obtainManager();
};

} // namespace aero
