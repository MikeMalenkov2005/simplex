

Data Stream Protocol
--------------------

The Data Stream Protocol (DSP) defines the general message structure utilized
for data streaming between tasks.

The DSP messages are divided in two mandatory and one optional parts:
1. a 4-byte message header
2. an optional 4-byte stream id
3. a message body (data)

The message header is used for providing the folowing information:
1. the presence of the optional stream id
2. the data direction for this message
3. a sequence number (to avoid sending stream id multiple times)

If a streaming task supports configuration messages the header should indicate
whether the message is a data message or a configuration message.

The stream id is used for distinguishing streams utilized by the same task.

Responses to the 

The general header format:
1. a 2-byte sequence number or zero
2. a 1-byte message flags (data direction or configuration, is stream id present, is response, and so on)
3. a 1-byte byte count (can not be greater then the maximum body size)

