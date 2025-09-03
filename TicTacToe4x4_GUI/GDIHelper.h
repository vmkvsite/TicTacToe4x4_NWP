#pragma once

#include <windows.h>

class GDIObject {
private:
    HGDIOBJ obj;

public:
    explicit GDIObject(HGDIOBJ o) : obj(o) {}
    ~GDIObject() {
        if (obj) DeleteObject(obj);
    }

    GDIObject(const GDIObject&) = delete;
    GDIObject& operator=(const GDIObject&) = delete;

    operator HGDIOBJ() const { return obj; }

    template<typename T>
    operator T() const { return static_cast<T>(obj); }
};