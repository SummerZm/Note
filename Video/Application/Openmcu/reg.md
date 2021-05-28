## **Opemcu reg framework**

### Registrar类
- AccountThread    -- <RegistrarAccount> accountList    【注册登录队列】
- ConnectionThread -- <RegistrarConnection> connectionList 【用户注册链接队列 - 在线程中更新链接状态】
- SubscriptionThread -- subscriptionList    【订阅列表】
- QueueThread  -- <MCUQueuePString> regQueue 【在线程中-处理注册流程操作 invite/establish/clear 】
- BookThread   -- <RegistrarAccount>【Add accountList成员 to abookList】 <abookList>【根据connectionlist更新abookList的状态; SendRoomControl】
- McuVerifyLB
- cpuThread
- HeartBeatsThread
- aliveThread -- SipSendPing() 【详见reg_sip.cxx】
- serverList
- isnatip [NAT]
- conn [MySql]