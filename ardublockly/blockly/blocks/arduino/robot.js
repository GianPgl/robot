'use strict';


/*Blockly.Blocks['robot_setup'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("Robot Setup:");
        this.appendDummyInput()
            .appendField("Speed")
            .appendField(new Blockly.FieldNumber(255), "SPPED")
            .appendField("(0 to 255)");
        this.appendDummyInput()
            .appendField("Name ")
            .appendField(new Blockly.FieldTextInput("robot"), "NAME");
        this.setColour(120);
        this.setTooltip("Set up your robot");
        this.setHelpUrl("");
    }
};*/

Blockly.Blocks['robot_get_delay'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("delay time");
        this.setOutput(true, "Number");
        this.setColour(230);
        this.setTooltip("time to accomplish a 90-degrees rotation");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_go'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("go")
            .appendField(new Blockly.FieldDropdown([["forward", "F"], ["backward", "B"]]), "DIR");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("Moves in the chosen direction");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_turn'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("turn")
            .appendField(new Blockly.FieldDropdown([["right", "R"], ["left", "L"]]), "DIR");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("Turns left or right");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_rotation'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("rotate")
            .appendField(new Blockly.FieldDropdown([["right", "R"], ["left", "L"]]), "DIR");
        this.appendDummyInput()
            .appendField("degrees")
            .appendField(new Blockly.FieldDropdown([["90", "90"], ["180", "180"], ["360", "360"], ["Other", "OTHER"]]), "DEG");
        this.appendDummyInput()
            .appendField("insert degrees (OTHER case)")
            .appendField(new Blockly.FieldNumber(0, 0), "OTHER_DEG");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_distance'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("read distance")
            .appendField(new Blockly.FieldDropdown([["cm", "CM"], ["in", "IN"]]), "UNIT");
        this.setOutput(true, "Number");
        this.setColour(230);
        this.setTooltip("reads distance from obstacles (if any)");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_medium_distance'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("read medium distance")
            .appendField(new Blockly.FieldDropdown([["cm", "CM"], ["in", "IN"]]), "UNIT");
        this.setOutput(true, "Number");
        this.setColour(230);
        this.setTooltip("reads medium distance from obstacles (if any)");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_is_clear'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("is clear");
        this.setOutput(true, "Boolean");
        this.setColour(230);
        this.setTooltip("checks for obstacles");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_find_safe_zone'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("find safe zone");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("finds a zone in wihich it can move");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_set_path'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("set path");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("finds ideal path");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_follow_line'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("follow line");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("follows a black line");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_get_line_dir'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("get line direction");
        this.setOutput(true, "Number");
        this.setColour(230);
        this.setTooltip("returns line direction (if any)");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_follow_avoid'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("follow line and avoid obstacles");
        this.appendDummyInput()
            .appendField("servo")
            .appendField(new Blockly.FieldDropdown([["fixed", "F"], ["moving", "M"]]), "SERVO");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("follows a black line and stops if it detects obstacles");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_set_servo'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("set servo state")
            .appendField(new Blockly.FieldDropdown([["on", "ON"], ["off", "OFF"]]), "STATE");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("servo on-off");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_get_light_dir'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("get light direction");
        this.appendValueInput("ENV_LIGHT")
            .setCheck("Number")
            .appendField("environement light value");
        this.setOutput(true, "Number");
        this.setColour(230);
        this.setTooltip("detects light direction given environement light");
        this.setHelpUrl("");
    }
};

Blockly.Blocks['robot_follow_light'] = {
    init: function () {
        this.appendDummyInput()
            .appendField("follow light");
        this.setPreviousStatement(true, null);
        this.setNextStatement(true, null);
        this.setColour(120);
        this.setTooltip("follows the light");
        this.setHelpUrl("");
    }
};