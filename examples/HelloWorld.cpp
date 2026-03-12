#include <aerolang/Activity.h>
#include <aerolang/View.h>
#include <aerolang/String.h>
#include <cstdint>

class MainActivity : public aero::Activity {
private:
    int32_t count = 0.000000;
    aero::String userName = aero::String("AeroLang User");

public:
    void onCreate() {
        count = 10.000000;
        showMessage(aero::String("Hello from AeroLang!"));
    }
    void showMessage(aero::String message) {
        return;
    }
};

// JNI Registration for MainActivity
AERO_REGISTER_ACTIVITY(MainActivity)

