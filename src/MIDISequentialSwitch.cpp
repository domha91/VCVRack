#include "plugin.hpp"


struct MIDISequentialSwitch : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		INPUT_1_INPUT,
		INPUT_9_INPUT,
		INPUT_2_INPUT,
		INPUT_10_INPUT,
		INPUT_3_INPUT,
		INPUT_11_INPUT,
		INPUT_4_INPUT,
		INPUT_12_INPUT,
		INPUT_5_INPUT,
		INPUT_13_INPUT,
		INPUT_6_INPUT,
		INPUT_14_INPUT,
		INPUT_7_INPUT,
		INPUT_15_INPUT,
		INPUT_8_INPUT,
		INPUT_16_INPUT,
		MIDI_INPUT_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHT_1_LIGHT,
		LIGHT_9_LIGHT,
		LIGHT_2_LIGHT,
		LIGHT_10_LIGHT,
		LIGHT_3_LIGHT,
		LIGHT_11_LIGHT,
		LIGHT_4_LIGHT,
		LIGHT_12_LIGHT,
		LIGHT_5_LIGHT,
		LIGHT_13_LIGHT,
		LIGHT_6_LIGHT,
		LIGHT_14_LIGHT,
		LIGHT_7_LIGHT,
		LIGHT_15_LIGHT,
		LIGHT_8_LIGHT,
		LIGHT_16_LIGHT,
		LIGHTS_LEN
	};

	MIDISequentialSwitch() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(INPUT_1_INPUT, "");
		configInput(INPUT_9_INPUT, "");
		configInput(INPUT_2_INPUT, "");
		configInput(INPUT_10_INPUT, "");
		configInput(INPUT_3_INPUT, "");
		configInput(INPUT_11_INPUT, "");
		configInput(INPUT_4_INPUT, "");
		configInput(INPUT_12_INPUT, "");
		configInput(INPUT_5_INPUT, "");
		configInput(INPUT_13_INPUT, "");
		configInput(INPUT_6_INPUT, "");
		configInput(INPUT_14_INPUT, "");
		configInput(INPUT_7_INPUT, "");
		configInput(INPUT_15_INPUT, "");
		configInput(INPUT_8_INPUT, "");
		configInput(INPUT_16_INPUT, "");
		configInput(MIDI_INPUT_INPUT, "");
		configOutput(OUTPUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
    // Default to the first input in case no valid MIDI note is received
    int selectedInput = 0;

    // Check if the MIDI input is connected and read the MIDI note value (assuming a 1V/octave standard for MIDI notes)
    if (inputs[MIDI_INPUT_INPUT].isConnected()) {
        float midiVoltage = inputs[MIDI_INPUT_INPUT].getVoltage();

        // Convert the MIDI voltage to a note value (60 for C4 to 75 for D#5)
        int midiNote = round(midiVoltage * 12.0f) + 60;  // 1V/octave means 1V = 12 semitones
        
        // Map the MIDI note to an input index (0 to 15)
        if (midiNote >= 60 && midiNote <= 75) {
            selectedInput = midiNote - 60;
        }
    }

    // Output the selected input's signal
    if (inputs[INPUT_1_INPUT + selectedInput].isConnected()) {
        outputs[OUTPUT_OUTPUT].setVoltage(inputs[INPUT_1_INPUT + selectedInput].getVoltage());
    } else {
        outputs[OUTPUT_OUTPUT].setVoltage(0.0f);  // Set to 0 if the selected input is not connected
    }

    // Update lights: Turn on the light for the selected input and turn off others
    for (int i = 0; i < 16; i++) {
        lights[LIGHT_1_LIGHT + i].setBrightness(i == selectedInput ? 1.0f : 0.0f);
    }
	}
};


struct MIDISequentialSwitchWidget : ModuleWidget {
	MIDISequentialSwitchWidget(MIDISequentialSwitch* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/MIDISequentialSwitch.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.077, 14.785)), module, MIDISequentialSwitch::INPUT_1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.09, 14.924)), module, MIDISequentialSwitch::INPUT_9_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.195, 27.481)), module, MIDISequentialSwitch::INPUT_2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.208, 27.619)), module, MIDISequentialSwitch::INPUT_10_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.45, 39.264)), module, MIDISequentialSwitch::INPUT_3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.463, 39.402)), module, MIDISequentialSwitch::INPUT_11_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.64, 50.243)), module, MIDISequentialSwitch::INPUT_4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.653, 50.382)), module, MIDISequentialSwitch::INPUT_12_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.668, 61.869)), module, MIDISequentialSwitch::INPUT_5_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.681, 62.007)), module, MIDISequentialSwitch::INPUT_13_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.786, 73.506)), module, MIDISequentialSwitch::INPUT_6_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.799, 73.644)), module, MIDISequentialSwitch::INPUT_14_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.042, 85.289)), module, MIDISequentialSwitch::INPUT_7_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.055, 85.427)), module, MIDISequentialSwitch::INPUT_15_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.232, 96.269)), module, MIDISequentialSwitch::INPUT_8_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.245, 96.407)), module, MIDISequentialSwitch::INPUT_16_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.86, 109.317)), module, MIDISequentialSwitch::MIDI_INPUT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(33.21, 109.536)), module, MIDISequentialSwitch::OUTPUT_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.05, 14.466)), module, MIDISequentialSwitch::LIGHT_1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(38.86, 14.585)), module, MIDISequentialSwitch::LIGHT_9_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.261, 27.373)), module, MIDISequentialSwitch::LIGHT_2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.072, 27.492)), module, MIDISequentialSwitch::LIGHT_10_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.493, 39.229)), module, MIDISequentialSwitch::LIGHT_3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.303, 39.348)), module, MIDISequentialSwitch::LIGHT_11_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.495, 50.346)), module, MIDISequentialSwitch::LIGHT_4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.305, 50.464)), module, MIDISequentialSwitch::LIGHT_12_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.508, 61.65)), module, MIDISequentialSwitch::LIGHT_5_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.318, 61.768)), module, MIDISequentialSwitch::LIGHT_13_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.72, 73.498)), module, MIDISequentialSwitch::LIGHT_6_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.53, 73.617)), module, MIDISequentialSwitch::LIGHT_14_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.951, 85.354)), module, MIDISequentialSwitch::LIGHT_7_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.761, 85.473)), module, MIDISequentialSwitch::LIGHT_15_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.953, 96.471)), module, MIDISequentialSwitch::LIGHT_8_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(39.763, 96.59)), module, MIDISequentialSwitch::LIGHT_16_LIGHT));
	}
};


Model* modelMIDISequentialSwitch = createModel<MIDISequentialSwitch, MIDISequentialSwitchWidget>("MIDISequentialSwitch");
