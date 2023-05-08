/*
 *
 * native.cc
 *
 * Created by Ruibin.Chow on 2023/05/03.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include <napi.h>
#include <assert.h>
#include <node_api.h>
#include <vector>

using namespace std;

static napi_value MyFun(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value str;
  status = napi_create_string_utf8(env, "mytest", 6, &str);
  assert(status == napi_ok);
  return str;
}

static napi_value YouFun(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 2;
  napi_value args[2];

  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  
  size_t strleng;
  napi_get_value_string_utf8(env, args[0], 0, 0, &strleng);
  string p1;
  p1.reserve(strleng + 1);
  p1.resize(strleng);
  status = napi_get_value_string_utf8(env, args[0], &p1[0], p1.capacity(), nullptr);

  napi_get_value_string_utf8(env, args[1], 0, 0, &strleng);
  string p2;
  p2.reserve(strleng + 1);
  p2.resize(strleng);
  status = napi_get_value_string_utf8(env, args[1], &p2[0], p2.capacity(), nullptr);
  napi_value v;
  string dest = "p1:"+ p1 + "p2:" + p2;
  napi_create_string_utf8(env,dest.c_str(), strlen(dest.c_str()+1), &v);
  return v;

}

#define DECLARE_NAPI_METHOD(name, func)                                        \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor mytest = DECLARE_NAPI_METHOD("mytest", MyFun);
  status = napi_define_properties(env, exports, 1, &mytest);
  napi_property_descriptor youtest = DECLARE_NAPI_METHOD("youtest", YouFun);
  status = napi_define_properties(env, exports, 1, &youtest);
  assert(status == napi_ok);
  return exports;
}

// NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

//------------------------------------------------------------------------------
class MyObject : public Napi::ObjectWrap<MyObject> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func =
        DefineClass(env,
                    "MyObject",
                    {InstanceMethod("plusOne", &MyObject::PlusOne),
                    InstanceMethod("value", &MyObject::GetValue),
                    InstanceMethod("multiply", &MyObject::Multiply)});

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("MyObject", func);
    return exports;
  }

  MyObject(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MyObject>(info) {
    Napi::Env env = info.Env();

    int length = info.Length();

    if (length <= 0 || !info[0].IsNumber()) {
      Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
      return;
    }

    Napi::Number value = info[0].As<Napi::Number>();
    this->value_ = value.DoubleValue();
  }

 private:
  Napi::Value GetValue(const Napi::CallbackInfo& info) {
    double num = this->value_;
    return Napi::Number::New(info.Env(), num);
  }

  Napi::Value PlusOne(const Napi::CallbackInfo& info) {
    this->value_ = this->value_ + 1;
    return MyObject::GetValue(info);
  }

  Napi::Value Multiply(const Napi::CallbackInfo& info) {
    Napi::Number multiple;
    if (info.Length() <= 0 || !info[0].IsNumber()) {
      multiple = Napi::Number::New(info.Env(), 1);
    } else {
      multiple = info[0].As<Napi::Number>();
    }

    Napi::Object obj = info.Env().GetInstanceData<Napi::FunctionReference>()->New(
        {Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue())});

    return obj;
  }

  double value_;
};

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return MyObject::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)




