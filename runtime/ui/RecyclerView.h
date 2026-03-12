#ifndef AERO_RECYCLER_VIEW_H
#define AERO_RECYCLER_VIEW_H

#include "View.h"
#include "../collections/List.h"
#include <jni.h>
#include <functional>

namespace aero {

// Adapter base class for RecyclerView
template<typename T>
class RecyclerViewAdapter {
public:
    RecyclerViewAdapter();
    virtual ~RecyclerViewAdapter();
    void setData(List<T>* newData);
    virtual int getItemCount() const;
    virtual jobject onCreateViewHolder(jobject parent, int viewType) = 0;
    virtual void onBindViewHolder(jobject holder, int position) = 0;

protected:
    // Internal data and JNI state omitted.
};

class RecyclerView : public View {
public:
    RecyclerView();
    void setLayoutManager(int type);
    template<typename T>
    void setAdapter(RecyclerViewAdapter<T>* newAdapter);
    void notifyDataSetChanged();

private:
    // Internal JNI bridge and adapter synchronization logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_RECYCLER_VIEW_H
