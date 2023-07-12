
## CppService

### 使用components.py创建组件

如：

```
components.py AudioDect
```

> 在components下生成组件目录与相关文件
> 
> 在shell生成组件的budiler目录与文件

**components.cmake与shell/ComponentFactory.cc会自动添加组件相关信息**

### 调用方式

通过组件接口调用

#### 外部调用

```cpp
std::shared_ptr<ServiceInterface> service = engine->GetService<ServiceInterface>(Protocol(ServiceInterface));
if (service != nullptr) {
    service->Description();
    service->Test();
}
```

#### 内部调用

```cpp
auto service = adapter_->GetEngine()->GetService<ServiceInterface>(Protocol(ServiceInterface));
service->GetTestString()
```

> 注意：**组件外部与内部都必须通过`GetService`模板方式获取**
