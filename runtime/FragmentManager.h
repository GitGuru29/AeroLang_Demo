#pragma once

#include <string>
#include <jni.h>
#include "Fragment.h"

namespace aero {

// Forward declaration
class FragmentTransaction;

// FragmentManager for managing fragments in an activity
class FragmentManager {
public:
    FragmentManager(JNIEnv* env, jobject managerInstance);
    ~FragmentManager();
    
    // Start a fragment transaction
    FragmentTransaction* beginTransaction();
    
    // Execute pending transactions
    bool executePendingTransactions();
    
    // Find fragments
    Fragment* findFragmentById(int id);
    Fragment* findFragmentByTag(const char* tag);
    
    // Back stack management
    int getBackStackEntryCount() const;
    void popBackStack();
    void popBackStackImmediate();
    bool popBackStackImmediate(const char* name, int flags);
    
    // Back stack flags
    static constexpr int POP_BACK_STACK_INCLUSIVE = 1;
    
protected:
    JNIEnv* env;
    jobject managerInstance;
};

// FragmentTransaction for adding/removing/replacing fragments
class FragmentTransaction {
public:
    FragmentTransaction(JNIEnv* env, jobject transactionInstance);
    ~FragmentTransaction();
    
    // Fragment operations
    FragmentTransaction* add(int containerViewId, Fragment* fragment);
    FragmentTransaction* add(int containerViewId, Fragment* fragment, const char* tag);
    FragmentTransaction* replace(int containerViewId, Fragment* fragment);
    FragmentTransaction* replace(int containerViewId, Fragment* fragment, const char* tag);
    FragmentTransaction* remove(Fragment* fragment);
    FragmentTransaction* show(Fragment* fragment);
    FragmentTransaction* hide(Fragment* fragment);
    FragmentTransaction* attach(Fragment* fragment);
    FragmentTransaction* detach(Fragment* fragment);
    
    // Back stack
    FragmentTransaction* addToBackStack(const char* name);
    
    // Transitions
    FragmentTransaction* setTransition(int transition);
    FragmentTransaction* setCustomAnimations(int enter, int exit);
    FragmentTransaction* setCustomAnimations(int enter, int exit, int popEnter, int popExit);
    
    // Standard transitions
    static constexpr int TRANSIT_NONE = 0;
    static constexpr int TRANSIT_FRAGMENT_OPEN = 1;
    static constexpr int TRANSIT_FRAGMENT_CLOSE = 2;
    static constexpr int TRANSIT_FRAGMENT_FADE = 3;
    
    // Commit transaction
    int commit();
    int commitAllowingStateLoss();
    void commitNow();
    void commitNowAllowingStateLoss();
    
private:
    JNIEnv* env;
    jobject transactionInstance;
};

} // namespace aero
