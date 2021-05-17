## **SDP [Session Description Protocol]**

### **1. 简介**
> **通过SDP配置属性和参数的过程称为：配置会话**
- SDP 用于描述多媒体会话的通告和邀请阶段；广泛应用于VOIP，视频会议。
- SDP 不用于传输多媒体流；主要用于终端之间网络，多媒体类型等其他属性的协商。
- SDP 起初是SAP[session announcement procotol]的一部分。
- SDP 可以和SIP[session initiation protocol]，RTP[real-time protocol]，RSTP[real-time streaming procotol]结合使用。
- SDP 后来作为单独的协议用于描述多播会议。

### **2. SDP组成**
- **Session description**
    ```sh
    v=  (protocol version number, currently only 0)
    o=  (originator and session identifier : username, id, version number, network address)
    s=  (session name : mandatory with at least one UTF-8-encoded character)
    i=* (session title or short information)
    u=* (URI of description)
    e=* (zero or more email address with optional name of contacts)
    p=* (zero or more phone number with optional name of contacts)
    c=* (connection information—not required if included in all media)
    b=* (zero or more bandwidth information lines)
    One or more Time descriptions ("t=" and "r=" lines; see below)
    z=* (time zone adjustments)
    k=* (encryption key)
    a=* (zero or more session attribute lines)
    Zero or more Media descriptions (each one starting by an "m=" line; see below)
    ```

- **Time description (mandatory)**
    ```sh
    t=  (time the session is active)
    r=* (zero or more repeat times)
    ```

- **Media description (optional)**
    ```sh
    m=  (media name and transport address)
    i=* (media title or information field)
    c=* (connection information — optional if included at session level)
    b=* (zero or more bandwidth information lines)
    k=* (encryption key)
    a=* (zero or more media attribute lines — overriding the Session attribute lines)
    ```

### **3. 例子简介**
- **SDP例子简介**
    ```sh
    # SDP协议版本
    v=0
    # 会话发起者信息
    o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
    # 会话名称
    s=SDP Seminar
    # 标题
    i=A Seminar on the session description protocol
    u=http://www.example.com/seminars/sdp.pdf
    # 响应方地址
    e=j.doe@example.com (Jane Doe)
    # 连接信息
    c=IN IP4 224.2.17.12/127
    # 会话活动时间[NTP时间戳](这里两者相减为2小时) 
    t=2873397496 2873404696
    # SDP拓展的属性
    a=recvonly
    # 媒体信息
    m=audio 49170 RTP/AVP 0
    m=video 51372 RTP/AVP 99
    a=rtpmap:99 h263-1998/90000
    ```

### **4. 属性拓展介绍**
- **A property form: a=flag conveys a simple boolean property of the media or session.**
- **A value form: a=attribute:value provides a named parameter.**
- **例子**
    ```sh
    # a=charset:encoding is used in the session or media sections to specify another character encoding (as registered in the IANA registry[6]) than the recommended default UTF-8 for standard protocol keys whose values are containing a text intended to be displayed to a user.

    # a=sdplang:code is used to specify in which language it is written, Alternate texts in multiple languages may be carried in the protocol, and selected automatically by the user agent according to user preferences.
    ```
- **进一步的解释**
    ```sh
    # 很重要的一个点
    # In both cases, textual fields intended to be displayed to a user will be interpreted as opaque strings, but rendered to the user or application with the values indicated in the last occurrence of the charset and sdplang in the current media section, or otherwise their last value in the session section.
    ```

### **5. 会话时间设置**
> **SDP可以用于设置定时会话【以下英语有点难懂】**

- **设置每周特定时间开启视频会议**
    ```sh
    Absolute times are represented in Network Time Protocol (NTP) format (the number of seconds since 1900). If the stop time is 0 then the session is "unbounded." If the start time is also zero then the session is considered "permanent." Unbounded and permanent sessions are discouraged but not prohibited. Intervals can be represented with NTP times or in typed time: a value and time units (days ('d'), hours ('h'), minutes ('m') and seconds ('s')) sequence.

    Thus an hour meeting from 10 am UTC on 1 August 2010, with a single repeat time a week later at the same time can be represented as:

        t=1280656800 1281265200
        r=604800 3600 0
    Or using typed time:

        t=1280656800 1281265200
        r=7d 1h 0
    ```

- **定期开启会议的时间，时区同步，和时间补偿问题【反正我是看不懂.语法太复杂了】**
    ```sh
    When repeat times are specified, the start time of each repetition may need to be adjusted so that it will occur at the same local time in a specific time zone throughout the period between the start time and the stop time (which are still specified in NTP format in UTC).

    Instead of specifying this time zone and having to support a database of time zones for knowing when and where daylight adjustments will be needed, the repeat times are assumed to be all defined within the same time zone, and SDP supports the indication of NTP absolute times when a daylight offset (expressed in seconds or using a type time) will need to be applied to the repeated start time or end time falling at or after each daylight adjustment. All these offsets are relative to the start time, they are not cumulative. NTP supports this with the z= field which indicates a series of pairs, whose first item is the NTP absolute time when a daylight adjustment will occur, and the second item indicates the offset to apply relative to the absolute times computed with the r= field.

    For example, if a daylight adjustment will subtract 1 hour on 31 October 2010 at 3 am UTC (i.e. 60 days minus 7 hours after the start time on Sunday 1 August 2010 at 10am UTC), and this will be the only daylight adjustment to apply in the scheduled period which would occur between 1 August 2010 up to the 28 November 2010 at 10 am UTC (the stop time of the repeated 1h session which is repeated each week at the same local time, which occurs 88 days later), this can be specified as:

        t=1280656800 1290938400
        r=7d 1h 0
        z=1288494000 -1h
    ```

    [相关链接](https://en.wikipedia.org/wiki/Session_Description_Protocol)






















