# Ccsds_Pkt
A project to create and send Spacewire packets based on the CCSDS protocol
<br>
Here is view of the general structure of a TC CCSDS packet.
<br><br>
![image](https://user-images.githubusercontent.com/46071575/171453553-367724dc-9633-4c5b-a365-0e787e8eba58.png)


# Example

This example shows the transmission of a TC packet from a spacewire port (3) of the GR740 board's router to another one (6).
The destination address in the spacewire header of the packet corresponds to an AMBA port of the router (port 11, logical address 0x2b=43).
This logical address allows to avoid header deletion so that the address will remain in the CCSDS packet.
<br>
```
***********  PKT TX/RX TEST  **************

SPW src port : 3
SPW dest port : 6
0 pkt(s) are (is) waiting for transmission

------ PKT 1 ------
-------------------
TX on GRSPW device 0 (AMBA port 1)

GRSPW0: Sending 1 packet(s) of length 518 bytes: 
 
 <><><> TX packet <><><>

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
|    Sec hdr ackFlag (1b)   |           2             
|  Sec hdr serviceType (1b) |           1             
|Sec hdr serviceSubType (1b)|           1             
|    Sec hdr sourceID (2b)  |           1             
|    Sec hdr spare (1b)     |           0             
|      App data 1 (1b)      |           0             
|      App data 2 (1b)      |           1             
|      App data 3 (1b)      |           2             
|                 (... 496 bytes of app data ...)       
|      App data 499 (1b)    |           9             
|           crc (2b)        |           77             
|______________________________________________________


GRSPW2: Receiving 1 packet(s) of length 519 bytes: 
 
 <><><> RX packet <><><>

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
|    Sec hdr ackFlag (1b)   |           2             
|  Sec hdr serviceType (1b) |           1             
|Sec hdr serviceSubType (1b)|           1             
|    Sec hdr sourceID (2b)  |           1             
|    Sec hdr spare (1b)     |           0             
|      App data 1 (1b)      |           0             
|      App data 2 (1b)      |           1             
|      App data 3 (1b)      |           2             
|                 (... 496 bytes of app data ...)       
|      App data 499 (1b)    |           9             
|           crc (2b)        |           77             
|______________________________________________________


END OF THE TEST: 1 packet(s) was (were) successfully sended and received.


```


