Drived Streaming Protocol
-------------------------

The Drived Streaming Protocol (DSP) defines the general message structure
utilized for data streaming between through driver tasks.

The general packet structure:
1. 2-byte stream/device id
2. 1-byte action/packet id
3. 1-byte the payload size
4. the payload data

The stream id is used to distinguish multiple streams opened for one task.

The payload data depends on the action id and the payload size.

The payload size can be used to indicate if an error has occured.

If the action is any type of request except read or write,
the payload size should be set to zero by the client and ignored by the driver.

If the action is a reply then the payload size specifies,
how many bytes was successfuly read or written by the driver.

All action ids from 0 upto 127 are reserved by the protocol.
Action ids from 128 upto 255 can be utilized for driver-specific actions.

