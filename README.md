# MHP Drivetrain/Rolling Resistance Rig

This repo stores code required for two testing setups.

1. Drivetrain Rig
- Here a power meter is connected to the crank and a rider pedals to drive a generator. 
- Power output on the generator is measured using Power = Voltage x Current.
- We use an arduino uno to measure the instantaneous voltage and current respectively
- This is transmitted over a serial link to a computer
- PLX-DAQ is used to capture the incoming serial data on the computer end, and log this to a spreadsheet.
  https://www.parallax.com/downloads/plx-daq

2. Rolling Resistance Rig
- Here a reed switch is connected to the fork. A magnet is attached to the wheel. A pi is connected to the reed switch.
- The pi runs a script on boot which monitors the push button. (To set this up the /etc/rc.local file and add the line 
  "python /home/pi/Documents/MHP_DTR/Raspi_ReedSwitch/run.py &" before "exit 0")
- When the push button is pressed the run.py script calls rs.py or rs_simple.py.
- rs.py captures zero crossings caused by the magnet passing the reed switch. The time delay between two falling edges, combined with
  knowledge of the wheel size (d=2*pi*r), allows us to calculate velocity of the wheel (distance/time). Velocity is stored to a csv file.
- rs_simple.py captures zero crossings caused by the magnet passing the reed switch. This script stores only the time delay between zero
  crossings. Speed is calculated in post analysis using excel.
- Files are transferred of the pi by using scp. A typical command could be "scp pi@192.168.100
  <INSERT>:/home/pi/Documents/MHP_DTR/Raspi_ReedSwitch/Recording_0.csv /Users/<INSERT>/Desktop" where <INSERT> specifies setup dependent
  variables. Note your computer will be need to be connected to MHP_MobileNet, as will the pi used.
