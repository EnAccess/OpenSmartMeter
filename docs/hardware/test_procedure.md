## TEST PROCEDURE


### I- System startup

1. Upload the code into the STM32
2. Connection of different components
3. System power
4. Start-up


### II- Configuration of the operating mode.
1. Enter setup mode
2. Choose the different operating modes (STS, OpayGo Time-based? OpayGo Energy-based)


### III- Entry of token For each mode

#### A. When recharging for the first time 
Test the entry of two types of tokens:

1. STS Mode
    a- a correct token => supposed to set credit (energy)
    b- an incorrect token => supposed to do nothing 

2. OpayGo Time-based Mode
    a- a correct token => supposed to set time
    b- an incorrect token => supposed to do nothing

3. OpayGo Energy-based Mode
    a- a correct token => supposed to set credit (energy)
    b- an incorrect token => supposed to do nothing

#### B. When the previous recharge is completed 
Test the entry of three types of tokens:

1. STS Mode
    a- a correct token => supposed to set a new credit (energy)
    b- an incorrect token => supposed to do nothing
    c- a token already used => supposed to do nothing

2. OpayGo Time-based Mode
    a- a correct token => supposed to set a new time
    b- an incorrect token => supposed to do nothing
    c- a token already => supposed to do nothing

3. OpayGo Energy-based Mode
    a- a correct token => supposed to set a new credit (energy)
    b- an incorrect token => supposed to do nothing
    c- a token already => supposed to do nothing
 
#### C. When the previous recharge is still in progress
Test the entry of three types of tokens:

1. STS Mode
    a- a correct token => supposed to add credit (energy) to the current value
    b- an incorrect token => supposed to do nothing
    c- a token already used => supposed to do nothing

2. OpayGo Time-based Mode
    a- a correct token => supposed to add time to the current value
    b- an incorrect token => supposed to do nothing
    c- a token already => supposed to do nothing

3. OpayGo Energy-based Mode
    a- a correct token => supposed to add credit (energy) to the current value
    b- an incorrect token => supposed to do nothing
    c- a token already => supposed to do nothing

 
### IV- Perform a value reset for each mode (Restoration of factory settings)

1. STS Mode 

2. OpayGo Time-based Mode 

3. OpayGo Energy-based Mode 

### V- Changing the configuration password
1. Enter setup mode using old password
2. Replace the old password with the new one
3. Test entering configuration mode using the old password => supposed to do nothing
3. Test entering configuration mode using the new password => supposed to work!
