goog.provide('Blockly.Arduino.robot');

goog.require('Blockly.Arduino');

var robot = {
    speed : 255,
    setup : function () {
        Blockly.Arduino.definitions_['define_robot'] = '#include "Robot.h"\n';
        Blockly.Arduino.definitions_['var_robot'] = 'Robot robot(' + robot.speed + ');\n';
        Blockly.Arduino.setups_['setup_robot'] = 'robot.init();\n';
    }
};

Blockly.Arduino['robot_get_delay'] = function (block) {
    robot.setup();
    var code = 'robot.getDelay()';
    return [code, Blockly.Arduino.ORDER_ATOMIC];
}

Blockly.Arduino['robot_go'] = function (block) {
    robot.setup();
    var dir = block.getFieldValue('DIR');
    var code;
    if(dir=='F')
        code = 'robot.goForward();\n';
    else
        code = 'robot.goBackward();\n';
    return code;
};

Blockly.Arduino['robot_turn'] = function (block) {
    robot.setup();
    var dir = block.getFieldValue('DIR');
    var code;
    if(dir=='R')
        code = 'robot.turnRight();\n';
    else
        code = 'robot.turnLeft();\n';
    return code;
};

Blockly.Arduino['robot_rotation'] = function (block) {
    robot.setup();
    var dir = block.getFieldValue('DIR');
    var deg = block.getFieldValue('DEG');
    var other_deg = block.getFieldValue('OTHER_DEG');
    var code, dir_code;
    if(dir == 'R')
        dir_code = 'RIGHT';
    else
        dir_code = 'LEFT';
    switch(deg){
        case '90':
            code = 'robot.rightAngleRotation('+ dir_code +');\n';
            break;
        case '180':
            code = 'robot.halfRotation('+ dir_code +');\n';
            break;
        case '360':
            code = 'robot.fullRotation('+ dir_code +');\n';
            break;
        case 'OTHER':
            code = 'robot.rotate(' + dir_code +', '+ other_deg +');\n';
            break;
    }
    return code;
};

Blockly.Arduino['robot_distance'] = function (block) {
    robot.setup();
    var unit = block.getFieldValue('UNIT');
    var code;
    if(unit == 'CM')
        code = 'robot.readDistanceCM()';
    else
        code = 'robot.readDistanceIN()';
    return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['robot_medium_distance'] = function (block) {
    robot.setup();
    var unit = block.getFieldValue('UNIT');
    var code;
    if (unit == 'CM')
        code = 'robot.mediumDistanceCM()';
    else
        code = 'robot.mediumDistanceIN()';
    return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['robot_is_clear'] = function (block) {
    robot.setup();
    var code = 'robot.isClear()';
    return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['robot_find_safe_zone'] = function (block) {
    robot.setup();
    var code = 'robot.findSafeZone();\n';
    return code;
};

Blockly.Arduino['robot_set_path'] = function (block) {
    robot.setup();
    var code = 'robot.setPath();\n';
    return code;
};

Blockly.Arduino['robot_follow_line'] = function (block) {
    robot.setup();
    var code = 'robot.followLine();\n';
    return code;
};

Blockly.Arduino['robot_get_line_dir'] = function (block) {
    robot.setup();
    var code = 'robot.getLineDir()';
    return [code, Blockly.JavaScript.ORDER_ATOMIC];
};

Blockly.Arduino['robot_follow_avoid'] = function (block) {
    robot.setup();
    var servo = block.getFieldValue('SERVO');
    var code;
    if(servo == 'F')
        code = 'robot.followAvoid();\n';
    else
        code = 'robot.followAvoidServo();\n';
    return code;
};

Blockly.Arduino['robot_set_servo'] = function (block) {
    robot.setup();
    var state = block.getFieldValue('STATE');
    if(state == 'ON')
        state = 'true';
    else
        state = 'false';
    var code = 'robot.setServo('+ state +');\n';
    return code;
};

Blockly.Arduino['robot_get_light_dir'] = function (block) {
    robot.setup();
    var env_light = Blockly.JavaScript.valueToCode(block, 'ENV_LIGHT', Blockly.JavaScript.ORDER_ATOMIC);
    var code = 'robot.getLightDir('+ env_light +')';
    return [code, Blockly.Arduino.ORDER_ATOMIC];
};

Blockly.Arduino['robot_follow_light'] = function (block) {
    robot.setup();
    var code = 'robot.followLight();\n';
    return code;
};