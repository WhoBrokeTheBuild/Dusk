#ifndef DUSK_EVENT_HPP
#define DUSK_EVENT_HPP

#include <dusk/Config.hpp>
#include <dusk/core/ScriptHost.hpp>

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

namespace dusk {

struct ICallback
{
public:

    virtual ~ICallback() = default;

}; // struct Callback

template <class ... Params>
class Event : public IScriptRef
{
public:

    typedef std::function<void(asIScriptContext*, Params ...)> ScriptPackFunc;

    ~Event() {
        for (auto cb : _callbacks) {
            cb.first->Detach();
        }
    }

    void Call(Params ... args) {
        _inCall = true;
        for (auto cb : _callbacks) {
            cb.first->Call(args...);
        }
        _inCall = false;
    }

    void RemoveICallback(ICallback * callback) {
        Callback * evtCb = dynamic_cast<Callback*>(callback);
        if (evtCb) {
            auto it = _callbacks.find(evtCb);
            if (it != _callbacks.end()) {
                _callbacks.erase(it);
                evtCb->Detach();
            }
        }
    }

    void AddICallback(ICallback * callback) {
        Callback * evtCb = dynamic_cast<Callback*>(callback);
        if (evtCb) {
            evtCb->Attach(this);
            _callbacks.emplace(evtCb, true);
        }
    }

    std::unique_ptr<ICallback> AddPassthrough(Event<Params ...> * event) {
        auto ptr = std::unique_ptr<ICallback>(new PassthroughCallback(event));
        AddICallback(ptr.get());
        return ptr;
    }

    std::unique_ptr<ICallback> AddStatic(std::function<void(Params ...)> func) {
        auto ptr = std::unique_ptr<ICallback>(new StaticCallback(func));
        AddICallback(ptr.get());
        return ptr;
    }

    template <class Object>
    std::unique_ptr<ICallback> AddMember(Object * object, std::function<void(Object *, Params ...)> func) {
        auto ptr = std::unique_ptr<ICallback>(new MemberCallback<Object>(object, func));
        AddICallback(ptr.get());
        return ptr;
    }

    std::unique_ptr<ICallback> AddScript(ScriptHost * host, std::string func, ScriptPackFunc packer) {
        auto ptr = std::unique_ptr<ICallback>(new ScriptCallback(host, func, packer));
        AddICallback(ptr.get());
        return ptr;
    }


private:

    struct Callback : public ICallback
    {
    public:

        Callback()
            : ICallback()
            , _event(nullptr)
        { }

        virtual ~Callback() {
            if (_event) {
                _event->RemoveICallback(this);
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

    }; // struct ICallback

    struct PassthroughCallback : public Callback
    {
    public:

        PassthroughCallback(Event<Params ...> * event)
            : Callback()
            , _nextEvent(event)
        { }

        void Call(Params ... args) override {
            _nextEvent->Call(args...);
        }

    private:

        Event<Params ...> * _nextEvent;

    }; // struct PassthroughCallback

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
    struct MemberCallback : public Callback
    {
    public:

        MemberCallback(Object * object, std::function<void(Object *, Params ...)> func)
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

    }; // struct MemberCallback

    struct ScriptCallback : public Callback
    {
    public:

        ScriptCallback(ScriptHost * host, std::string func, ScriptPackFunc packer)
            : Callback()
            , _host(host)
            , _packer(packer)
        {
            _asFunc = _host->GetEngine()->GetModule(0)->GetFunctionByName(func.c_str());
        }

        void Call(Params ... args) override {
            asIScriptContext * asCtx = _host->GetContextFromPool();

            asCtx->Prepare(_asFunc);
            _packer(asCtx, args...);
            asCtx->Execute();

            _host->GiveContextToPool(asCtx);
        }

    private:

        ScriptHost * _host;
        asIScriptFunction * _asFunc;
        ScriptPackFunc _packer;

    }; // struct ScriptCallback

    bool _inCall = false;
    std::unordered_map<Callback*, bool> _callbacks;

}; // class Event

class ICallbackHost
{
public:

    virtual ~ICallbackHost() { }

    void TrackCallback(std::unique_ptr<ICallback>&& ptr) {
        _callbacks.push_back(std::move(ptr));
    }

    void ClearCallbacks() {
        _callbacks.clear();
    }

protected:

    std::vector<std::unique_ptr<ICallback>> _callbacks;

}; // class IICallbackHost

} // namespace dusk

#endif // DUSK_EVENT_HPP
