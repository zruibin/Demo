import React from 'react'
import '../assets/css/App.css'
// import logger from '../../log'
import { Button, message, notification } from 'antd';


function App() {
  // logger.debug("[App]", "App Load.");

  const [messageApi, messageHolder] = message.useMessage();
  const info = () => {
    messageApi.info("Electron Demo");
  };

  const [notificationApi, notificationHolder] = notification.useNotification();
  const openNotification = () => {
    notificationApi.open({
      message: 'Notification Title',
      description:
        'I will never close automatically. This is a purposely very very long description that has many many characters and words.',
      duration: 3.5,
    });
  };

  return (
    <div>
      <h1>Hello, Electron Demo!</h1>

      <p>I hope you enjoy using electron demo to start your dev off right!</p>
      {messageHolder}
      <Button type="primary" onClick={info}>Alert</Button>&nbsp;

      {notificationHolder}
      <Button type="primary" onClick={openNotification}>Notification</Button>
    </div>
  )
}

export default App
