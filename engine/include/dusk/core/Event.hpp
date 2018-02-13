#ifndef DUSK_EVENT_HPP
#define DUSK_EVENT_HPP

#include <dusk/Config.hpp>

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace dusk {

struct CallbackBase
{
public:

    virtual ~CallbackBase() { }

}; // struct CallbackBase

template <class ... Params>
class Event
{
public:

    ~Event() {
        for (auto cb : _callbacks) {
            cb.first->Detach();
        }
    }

    void Call(Params ... args) {
        for (auto cb : _callbacks) {
            cb.first->Call(args...);
        }
    }

    void RemoveCallbackBase(CallbackBase * callback) {
        Callback * evtCb = dynamic_cast<Callback*>(callback);
        if (evtCb) {
            auto it = _callbacks.find(evtCb);
            if (it != _callbacks.end()) {
                _callbacks.erase(it);
                evtCb->Detach();
            }
        }
    }

    void AddCallbackBase(CallbackBase * callback) {
        Callback * evtCb = dynamic_cast<Callback*>(callback);
        if (evtCb) {
            evtCb->Attach(this);
            _callbacks.emplace(evtCb, true);
        }
    }

    std::unique_ptr<CallbackBase> AddStatic(std::function<void(Params ...)> func) {
        auto ptr = std::unique_ptr<CallbackBase>(new StaticCallback(func));
        AddCallbackBase(ptr.get());
        return ptr;
    }

    template <class Object>
    std::unique_ptr<CallbackBase> AddMethod(Object * object, std::function<void(Object *, Params ...)> func) {
        auto ptr = std::unique_ptr<CallbackBase>(new MethodCallback<Object>(object, func));
        AddCallbackBase(ptr.get());
        return ptr;
    }


private:

    struct Callback : public CallbackBase
    {
    public:

        Callback()
            : CallbackBase()
            , _event(nullptr)
        { }

        virtual ~Callback() {
            if (_event) {
                _event->RemoveCallbackBase(this);
            }
        }

        void Attach(Event<Params ...> * event) {
            _event = event;
        }

        void Detach() {
            _event = nullptr;
        }

        virtual void Call(Params ... args) = 0;

    private:

        Event<Params ...> * _event;

    }; // struct CallbackBase

    struct StaticCallback : public Callback
    {
    public:

        StaticCallback(std::function<void(Params ...)> func)
            : Callback()
            , _callback(func)
        { }

        void Call(Params ... args) override {
            _callback(args...);
        }

    private:

        std::function<void(Params ...)> _callback;

    }; // struct StaticCallback

    template <class Object>
    struct MethodCallback : public Callback
    {
    public:

        MethodCallback(Object * object, std::function<void(Object *, Params ...)> func)
            : Callback()
            , _object(object)
            , _callback(func)
        { }

        void Call(Params ... args) override {
            _callback(_object, args...);
        }

    private:

        Object * _object;
        std::function<void(Object *, Params ...)> _callback;

    }; // struct MethodCallback

    std::unordered_map<Callback*, bool> _callbacks;

}; // class Event

class ICallbackHost
{
public:

    virtual ~ICallbackHost() { }

    void TrackCallback(std::unique_ptr<CallbackBase>&& ptr) {
        _callbacks.push_back(std::move(ptr));
    }

    void ClearCallbacks() {
        _callbacks.clear();
    }

protected:

    std::vector<std::unique_ptr<CallbackBase>> _callbacks;

}; // class ICallbackBaseHost

} // namespace dusk

#endif // DUSK_EVENT_HPP
