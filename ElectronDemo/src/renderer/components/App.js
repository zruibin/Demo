import React from 'react'
import '../assets/css/App.css'
import logger from '../log'
import { Button, message, notification } from 'antd';
import emitter from "../eventbus"

class App extends React.Component {
  constructor(props) {
    super(props);
    logger.info("[App]", "App Load.");

    this.state = {
      title: "Hello, Electron Demo!",
      detail: "I hope you enjoy using electron demo to start your dev off right!"
    };

    // 为了在回调中使用 `this`，这个绑定是必不可少的
    this.info = this.info.bind(this);
    this.openNotification = this.openNotification.bind(this);
  }

  static getDerivedStateFromProps(props, state){
    logger.debug("[App]", "执行getDerivedStateFromProps");
    return null;
  }

  componentDidMount(){
    logger.debug("[App]", "执行componentDidMount");
    emitter.on("app", (arg) => {
      logger.debug("[App]", "emitter.on:", arg);
    });
  }

  componentWillUnmount() {
    logger.debug("[App]", "componentWillUnmount");
    emitter.off("app", null);
  }

  shouldComponentUpdate(nextProps, nextState) {
    logger.debug("[App]", "shouldComponentUpdate");
    if (this.state.title !== nextState.title) {
      return true;
    }
    return false;
  }

  getSnapshotBeforeUpdate(prevProps, prevState) {
    logger.debug("[App]", "getSnapshotBeforeUpdate");
    return null;
  }

  componentDidUpdate(prevProps, prevState){
    logger.debug("[App]", "componentDidUpdate");
    if (prevState.title !== this.state.title) {
      logger.debug("[App]", "state中的info数据改变了");
    }
  }
  
  info() {
    message.info("Electron Demo");
    logger.debug("[App]", "Alert.");
    emitter.emit('app', "haha.");

    /*
    执行this.setState()并不总是立即更新组件，它会批量推迟更新。
    这使得在调用this.setState()后立即读取this.state成为了隐患。
    所以this.setState的第二个参数callback为可选的回调函数，在回调函数去读取更新后的state。
    */
    this.setState((state,props) => ({
      title: 'Hello, Electron And React!',
    }), () =>{
      logger.debug("[App]", this.state.title)
    });
  };

  openNotification() {
    notification.open({
      message: 'Notification Title',
      description:
        'I will never close automatically. This is a purposely very very long description that has many many characters and words.',
      duration: 3.5,
      onClick: () => {
        console.log('Notification Clicked!');
      },
    });
  };

  render() {
    logger.debug("[App]", "render");
    return (
      <div>
        <h1>{this.state.title}</h1>
        <p>{this.state.detail}</p>

        <Button type="primary" onClick={this.info}>Alert</Button>
        &nbsp;
        <Button type="primary" onClick={this.openNotification}>Notification</Button>
      </div>
    );
  }
}

export default App;