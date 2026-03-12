#ifndef AERO_SPINNER_H
#define AERO_SPINNER_H

#include "View.h"
#include "../collections/List.h"
#include <jni.h>
#include <functional>
#include <string>

namespace aero {

public:
    Spinner();
    void setItems(List<std::string>* itemList);
    void setSelection(int position);
    int getSelectedItemPosition() const;
    void setOnItemSelectedListener(std::function<void(int)> listener);

private:
    // Internal JNI bridge and adapter management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_SPINNER_H
