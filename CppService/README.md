
# CppService

## 架构

```
 -----------------------------------------------------------
|                        CaseEngine                        |
 -----------------------------------------------------------
 -------   -------   -------   -------   ------
| case1 | | case2 | | case3 | | case4 | | .... |
 -------   -------   -------   -------   ------
 -----------------------------------------------------------
|                      ServiceEngine                        |
 -----------------------------------------------------------
 ----------   ----------   ----------   ----------   ------
| service1 | | service2 | | service3 | | service4 | | .... |
 ----------   ----------   ----------   ----------   ------
```

> **Case可以依赖(包含)与获取到Service，Service则不可以依赖(包含)与获取到Case，Service的粒度最小！！！**

## Service

### 使用components.py创建Service组件

如：

```
components.py -c AudioDect
```

> 在components下生成组件目录与相关文件
> 
> 在shell生成组件的budiler目录与文件

**shell/ComponentFactory.cc会自动添加组件相关信息**

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
if (service != nullptr) {
    service->GetTestString()
}
```

> 注意：**组件外部与内部都必须通过`GetService`模板方式获取**

#### 创建组件Service

```cpp
std::shared_ptr<ServiceInterface> tmpService = engine->CreateService<ServiceInterface>(Protocol(ServiceInterface));
if (tmpService != nullptr) {
    tmpService->Init();
    tmpService->Description();
    tmpService->Destory();
}
```

> 注意：**通过`CreateService`方法获取到的Service生命周期由调用者操控，Service的`Init`与`Destory`也必须由调用者显式调用！**

## Case

### 使用cases.py创建Case组件

如：

```
cases.py -c TestCase
```

> 在components下生成组件目录与相关文件
> 
> 在shell生成组件的budiler目录与文件

**shell/CaseFactory.cc会自动添加组件相关信息**

### 调用方式

通过组件接口调用

#### 外部调用

```cpp
auto testCase = caseEngine->GetCase<TestCaseInterface>(Protocol(TestCaseInterface));
    if (testCase != nullptr) {
        std::cout << testCase->Description() << std::endl;
}
```

#### 内部调用

```cpp
auto testCase = adapter_->GetCaseEngine()->GetCase<TestCaseInterface>(Protocol(TestCaseInterface));
if (testCase != nullptr) {
    testCase->Description()
}
```

> 注意：**组件外部与内部都必须通过`GetCase`模板方式获取**

#### 创建组件Case

```cpp
std::shared_ptr<TestCaseInterface> tmpCase = caseEngine->CreateCase<TestCaseInterface>(Protocol(TestCaseInterface));
if (tmpCase != nullptr) {
    tmpCase->Init();
    tmpCase->Description();
    tmpCase->Destory();
}
```

> 注意：**通过`CreateCase`方法获取到的Case生命周期由调用者操控，Case的`Init`与`Destory`也必须由调用者显式调用！**