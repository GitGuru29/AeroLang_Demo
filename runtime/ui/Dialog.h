#ifndef AERO_DIALOG_H
#define AERO_DIALOG_H

#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class AlertDialog {
public:
    class Builder {
    public:
        Builder();
        Builder& setTitle(const char* title);
        Builder& setMessage(const char* message);
        Builder& setPositiveButton(const char* text, std::function<void()> callback);
        Builder& setNegativeButton(const char* text, std::function<void()> callback);
        AlertDialog* create();
        void show();

    private:
        // Internal JNI builder state omitted due to high-security proprietary details.
    };

    void show();
    void dismiss();

private:
    AlertDialog();
    // Internal JNI dialog instance omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_DIALOG_H
