# Mini_fpv_drone
A mini budget quadcopter drone designed to fly using commands from a pc (or any other input device that is connected to the reciever on the other end,
e.g remote control).
The body of the drone uses handcrafted wood carved from a printed out schematic, the motor driver is a hand-soldered circuit using 
schotky diodes, SMD mosfets & SMD resistors, for the propellers i used plastic 30 mm blades, the motors are 720 coreless motors, and the electronics also include:
one arduino pro mini 8MHz 3.3V that acts as the onboard computer, one NRF24L01 as a transciever, one MPU6050 sensor, one 3.7V 500mAh battery to power everything,
and a couple of bypass/decoupling capacitors, also there is an NRF24L01 and an arduino UNO R4 on the side of the PC to recieve,transmit,and analyze data and commands. 
The software should also be coded from scratch using the two arduinos as computers and another PC if required, 
in order to make all flight control and data handling algorithms without premade software. 
