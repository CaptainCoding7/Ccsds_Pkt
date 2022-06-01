# Ccsds_Pkt
A project to create and send Spacewire packets based on the CCSDS protocol
<br>
Here is view of the general structure of a CCSDS packet.
<br>
![image](https://user-images.githubusercontent.com/46071575/171451983-9e680a4a-d8eb-41d6-af75-bcb65595b47e.png)


# Example

This example show the transmission of a packet from a spacewire port of the GR740 board's router to an Amba port of the router (port 3 -> amba port 11).
The logical address used to select the destination port doesn't perform header deletion so that the address will remain in the CCSDS packet.
(At this point, the secondary header's fields doesn't contain the right values).
<br>
```
***********  PKT TX/RX TEST  **************

SPW src port : 3
SPW dest port : 43
1 pkts are waiting for transmission

------ PKT 1 ------
-------------------
TX on GRSPW device 0 (AMBA port 1)
GRSPW0: Sending 1 packets
 PKT of length 15 bytes:  0x03 0x2b 0x02 0x00 0x00 0x1a 0x40 0xc0 0x01 0x00 0x1e 0x00 0x00 0x00 0x00

 ______________________________________________________
| --------------------  CCSDS packet  -----------------
|           Field           |          Value           
|______________________________________________________
|     Spw hdr addr (1b)     |           43             
|     Spw hdr protid (1b)   |           2             
|     Spw hdr spare (1b)    |           0             
|     Spw hdr user_app (1b) |           0             
|        Prim hdr ID (2b)   |           6720             
|    Prim hdr seqCount (2b) |           1             
|        Prim hdr len (2b)  |           30             
|    Sec hdr sourceID (1b)  |           1             
|Sec hdr serviceSubType (1b)|           1             
|  Sec hdr serviceType (1b) |           1             
|    Sec hdr ackFlag (1b)   |           0             
|______________________________________________________

GRSPW2: Received 1 packets
 PKT of length 14 bytes: 0x2b 0x02 0x00 0x00 0x1a 0x40 0xc0 0x01 0x00 0x1e 0x00 0x00 0x00 0x00 

 ______________________________________________________
| --------------------  CCSDS packet  -----------------
|           Field           |          Value           
|______________________________________________________
|     Spw hdr addr (1b)     |           43             
|     Spw hdr protid (1b)   |           2             
|     Spw hdr spare (1b)    |           0             
|     Spw hdr user_app (1b) |           0             
|        Prim hdr ID (2b)   |           6720             
|    Prim hdr seqCount (2b) |           1             
|        Prim hdr len (2b)  |           30             
|    Sec hdr sourceID (1b)  |           0             
|Sec hdr serviceSubType (1b)|           0             
|  Sec hdr serviceType (1b) |           0             
|    Sec hdr ackFlag (1b)   |           0             
|______________________________________________________

```


