#ifndef OBJECT_H
#define OBJECT_H

#include <atomic>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <map>

#include "property.hpp"


#define PROP(cls, type, name) \
    public: \
    const type& name() const { return m_##name; } \
    cls* set_##name(const type& v) { if(m_##name != v) { m_##name = v; emit(#name "_changed", &m_##name); } return this; }\
    protected: type m_##name \
/*
#define PROPERTY_PTR(type, name) \
    public: \
    type name() { return m_##name; } \
    void set_##name(type v) { m_##name = v; }\
    protected: type m_##name \
*/

#define PROPERTY(type, name) Property<type> name

class Object {
public:
    /// Default constructor
    Object() { }

    /// Copy constructor
    Object(const Object &) : m_refCount(0) {}

    /// Return the current reference count
    int getRefCount() const { return m_refCount; }

    /// Increase the object's reference count by one
    void incRef() const { ++m_refCount; }

    /** \brief Decrease the reference count of
     * the object and possibly deallocate it.
     *
     * The object will automatically be deallocated once
     * the reference count reaches zero.
     */
    void decRef(bool dealloc = true) const noexcept;

    /*template <typename T, typename ...Args>
    int connect_member(std::string signal, T *inst, void (T::*func)(Args...)) {
      return connect(signal, [=](Args... args) {
        (inst->*func)(args...);
      });
    }

    template <typename T, typename ...Args>
      int connect_member(std::string signal, T *inst, void (T::*func)(Args...) const) {
        return connect(signal, [=](Args... args) {
          (inst->*func)(args...);
        });
      }*/

    template <typename ...Args>
    void connect(std::string signal, std::function<void(Args...)> const& slot) const {
        if(slots_.find(signal) == slots_.end()) {
            auto slots = std::vector<std::function<void(void*)>>();
            slots.push_back(slot);
            slots_[signal] = slots;
        } else {
            const auto &_slots = slots_.at(signal);
            _slots.push_back(slot);
        }
    }

    //template<typename arg>
    void emit(std::string signal, void* p) {
        //std::cout << signal << typeid(*p).name()  << std::endl;
        if(slots_.find(signal) == slots_.end())
            return;
        const auto &_slots = slots_.at(signal);
        for(const auto &it : _slots) {
            it(p);
        }
    }
protected:
    /** \brief Virtual protected deconstructor.
     * (Will only be called by \ref ref)
     */
    virtual ~Object();
private:
    mutable std::atomic<int> m_refCount { 0 };
    mutable std::map<std::string, std::vector<std::function<void(void*)>> > slots_;
};


#endif // OBJECT_H
