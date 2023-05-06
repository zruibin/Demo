import React from 'react'
import '../assets/css/App.css'
import logger from '../log'
import { Button, message, notification } from 'antd';

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
  
  info() {
    message.info("Electron Demo");
    logger.debug("[App]", "Alert.");

    this.setState(state => ({
      title: 'Hello, Electron And React!',
    }));
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