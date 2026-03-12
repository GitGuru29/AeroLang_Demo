# Background Processing in AeroLang

AeroLang provides comprehensive background processing capabilities for Android development, wrapping the native Android APIs through a C++ JNI bridge.

## Table of Contents
1. [Services](#services)
2. [WorkManager](#workmanager)
3. [BroadcastReceiver](#broadcastreceiver)
4. [Notifications](#notifications)
5. [AsyncTask & Threading](#asynctask--threading)

---

## Services

Services run in the background without a user interface. AeroLang supports both **started** and **bound** services.

### Declaring a Service

```aero
service DataSyncService {
    var syncInterval: Int = 30000
    
    fun onCreate() {
        showMessage("Service created")
    }
    
    fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int {
        syncData()
        return 1  // START_STICKY
    }
    
    fun syncData() {
        // Background work here
    }
    
    fun onDestroy() {
        showMessage("Service stopped")
    }
}
```

### Service Lifecycle Methods

| Method | Description |
|--------|-------------|
| `onCreate()` | Called once when the service is first created |
| `onStartCommand()` | Called each time the service is started via `startService()` |
| `onBind()` | Called when a component binds to the service |
| `onUnbind()` | Called when all clients disconnect |
| `onDestroy()` | Called when the service is being destroyed |

### Start Flags

| Value | Constant | Behavior |
|-------|----------|----------|
| 0 | `START_NOT_STICKY` | Don't recreate unless pending intents |
| 1 | `START_STICKY` | Recreate with null intent |
| 2 | `START_REDELIVER_INTENT` | Recreate with last delivered intent |

### Foreground Services

```cpp
// In C++ runtime code
service->startForeground(notificationId, notification);
service->stopForeground(true); // Remove notification when done
```

---

## WorkManager

WorkManager handles deferrable, guaranteed background work using the AndroidX WorkManager library.

### Creating a Worker

```cpp
class SyncWorker : public aero::Worker {
public:
    aero::WorkResult doWork() override {
        // Do background work
        return aero::WorkResult::SUCCESS;
    }
};
AERO_REGISTER_WORKER(SyncWorker)
```

### Scheduling Work

```cpp
using namespace aero;

// One-time request
WorkRequest request = WorkRequest::oneTime("SyncWorker")
    .setInitialDelay(15, TimeUnit::MINUTES)
    .addTag("sync")
    .build();

WorkManager::getInstance().enqueue(request);

// Periodic request (minimum 15 minutes)
WorkRequest periodic = WorkRequest::periodic("SyncWorker", 60) // minutes
    .addTag("periodic-sync")
    .build();

WorkManager::getInstance().enqueue(periodic);
```

### Constraints

```cpp
Constraints constraints;
constraints.setRequiredNetworkType(NetworkType::CONNECTED)
           .setRequiresCharging(true)
           .setRequiresDeviceIdle(false);

WorkRequest request = WorkRequest::oneTime("SyncWorker")
    .setConstraints(constraints)
    .build();
```

### Work Policies

```cpp
// Unique work — only one instance at a time
WorkManager::getInstance().enqueueUniqueWork(
    "sync-unique", ExistingWorkPolicy::KEEP, request);
```

---

## BroadcastReceiver

Listen for system and custom broadcast events.

### Creating a Receiver

```cpp
class NetworkReceiver : public aero::BroadcastReceiver {
public:
    void onReceive(JNIEnv* env, jobject context, aero::Intent* intent) override {
        // Handle network state change
    }
};
AERO_REGISTER_RECEIVER(NetworkReceiver)
```

### Lambda Receivers

```cpp
auto receiver = new aero::LambdaBroadcastReceiver(
    [](JNIEnv* env, jobject ctx, aero::Intent* intent) {
        // Handle broadcast inline
    }
);
```

### Registering Receivers

```cpp
aero::IntentFilter filter(aero::BroadcastReceiver::ACTION_CONNECTIVITY_CHANGE);
filter.addAction(aero::BroadcastReceiver::ACTION_BATTERY_LOW);

broadcastManager.registerReceiver(receiver, filter);
```

### Available System Actions

| Action | Description |
|--------|-------------|
| `ACTION_BOOT_COMPLETED` | Device finished booting |
| `ACTION_BATTERY_LOW` | Battery level is low |
| `ACTION_CONNECTIVITY_CHANGE` | Network connectivity changed |
| `ACTION_SCREEN_ON/OFF` | Screen turned on/off |
| `ACTION_PACKAGE_ADDED/REMOVED` | App installed/uninstalled |

---

## Notifications

Full notification support with channels (Android 8.0+) and a builder pattern.

### Creating a Channel

```cpp
aero::NotificationChannel channel("downloads", "Downloads",
    aero::NotificationChannel::IMPORTANCE_DEFAULT);
channel.setDescription("File download notifications")
       .enableVibration(true);

notificationManager.createChannel(channel);
```

### Building Notifications

```cpp
aero::NotificationBuilder builder(env, context, "downloads");
builder.setTitle("Download Complete")
       .setContent("photo.jpg saved to Downloads")
       .setSmallIcon(R_drawable_ic_download)
       .setAutoCancel(true)
       .setPriority(aero::NotificationBuilder::PRIORITY_DEFAULT);

notificationManager.notify(1, builder);
```

### Progress Notifications

```cpp
builder.setProgress(100, 45, false); // max, current, indeterminate
notificationManager.notify(1, builder);

// When done
builder.setProgress(0, 0, false);
builder.setContent("Download complete");
notificationManager.notify(1, builder);
```

### Notification Actions

```cpp
builder.addAction(R_drawable_ic_pause, "Pause", pauseIntent);
builder.addAction(R_drawable_ic_cancel, "Cancel", cancelIntent);
```

---

## AsyncTask & Threading

### AsyncTask Template

```cpp
class DownloadTask : public aero::AsyncTask<std::string> {
public:
    void onPreExecute() override {
        // Runs on UI thread before background work
    }
    
    std::string doInBackground() override {
        // Heavy work on background thread
        publishProgress(50);
        return "result";
    }
    
    void onPostExecute(const std::string& result) override {
        // Runs on UI thread with result
    }
    
    void onProgressUpdate(int progress) override {
        // Runs on UI thread with progress
    }
};

DownloadTask task;
task.execute();
```

### Convenience Functions

```cpp
// Run on background thread
aero::runAsync([]() {
    // Background work
});

// Run with delay
aero::delay(2000, []() {
    // Runs after 2 seconds
});
```

### Timer

```cpp
aero::Timer timer;
timer.setInterval(1000)
     .setCallback([]() {
         // Called every second
     });
timer.start();
// ...
timer.stop();
```

### Handler

```cpp
aero::Handler handler(env);
handler.post([]() {
    // Runs on main thread
});
handler.postDelayed([]() {
    // Runs after delay on main thread  
}, 5000);
```

---

## Architecture

All background processing components follow the same JNI bridge pattern:

```
AeroLang Code → C++ Runtime → JNI Bridge → Android Java APIs
```

Each component includes:
- **Registry pattern** for dynamic instance creation
- **Factory macros** (`AERO_REGISTER_SERVICE`, `AERO_REGISTER_WORKER`, etc.)
- **Lifecycle management** via JNI callbacks
- **Builder patterns** for complex configurations
