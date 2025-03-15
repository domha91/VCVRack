#include "beatSlicer.hpp"
#include <cmath>
#include <algorithm>

// Constructor
BeatSlicer::BeatSlicer() {
    // Configure the module with the number of parameters, inputs, outputs, and lights
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

    // Configure parameters
    configParam(STRETCH_KNOB_PARAM, 0.f, 1.f, 0.5f, "Stretch");
    configParam(SLICE_KNOB_PARAM, 0.f, 1.f, 0.f, "Slice Selector");
    configParam(RECORD_BUTTON_PARAM, 0.f, 1.f, 0.f, "Record Button");
    configParam(ENV_KNOB_PARAM, 0.f, 1.f, 0.5f, "Envelope");
    configParam(MODE_SWITCH_PARAM, 0.f, 2.f, 0.f, "Mode Switch");
}

// Pitch detection algorithm (simplified YIN)
float BeatSlicer::detectPitch(const std::vector<float>& buffer, size_t start, size_t end) {
    // Placeholder for pitch detection logic
    return 440.0f; // Default to A4
}

// Map pitch to microtonal scale
float BeatSlicer::mapPitchToScale(float pitch, const std::string& scale) {
    // Placeholder for microtonal mapping logic
    return pitch;
}

// Automatically detect slices based on transients
void BeatSlicer::autoDetectSlices() {
    slicePoints.clear();
    if (audioBufferLeft.empty()) return;

    // Simple transient detection (placeholder)
    float threshold = 0.1f;
    for (size_t i = 1; i < audioBufferLeft.size(); i++) {
        if (std::abs(audioBufferLeft[i] - audioBufferLeft[i - 1]) > threshold) {
            slicePoints.push_back(i);
        }
    }
}

// Process function
void BeatSlicer::process(const ProcessArgs &args) {
    // Handle recording
    if (params[RECORD_BUTTON_PARAM].getValue() > 0.f && !isRecording) {
        isRecording = true;
        audioBufferLeft.clear();
        audioBufferRight.clear();
        lights[RECORD_LIGHT_LIGHT].setBrightness(1.f);
    } else if (params[RECORD_BUTTON_PARAM].getValue() == 0.f && isRecording) {
        isRecording = false;
        lights[RECORD_LIGHT_LIGHT].setBrightness(0.f);
        autoDetectSlices(); // Detect slices after recording
    }

    if (isRecording) {
        // Record audio
        float leftInput = inputs[AUDIO_INPUT_LEFT_INPUT].getVoltage() / 10.f;
        float rightInput = inputs[AUDIO_INPUT_RIGHT_INPUT].getVoltage() / 10.f;
        audioBufferLeft.push_back(leftInput);
        audioBufferRight.push_back(rightInput);
    }

    // Handle playback
    if (audioBufferLeft.empty() || audioBufferRight.empty() || slicePoints.empty()) {
        outputs[AUDIO_OUTPUT_LEFT_OUTPUT].setVoltage(0.f);
        outputs[AUDIO_OUTPUT_RIGHT_OUTPUT].setVoltage(0.f);
        return;
    }

    float sliceSelector = params[SLICE_KNOB_PARAM].getValue();

    currentSlice = rack::math::clamp(
        static_cast<int>(sliceSelector * (slicePoints.size() - 1)), 
        0, 
        static_cast<int>(slicePoints.size() - 1)
    );

    size_t sliceStart = slicePoints[currentSlice];
    size_t sliceEnd = (currentSlice + 1 < slicePoints.size()) ? slicePoints[currentSlice + 1] : audioBufferLeft.size();
    sliceEnd = std::min(sliceEnd, audioBufferLeft.size()); // Prevent buffer overflow

    for (size_t i = sliceStart; i < sliceEnd; i++) {
        outputs[AUDIO_OUTPUT_LEFT_OUTPUT].setVoltage(audioBufferLeft[i] * 10.f);
        outputs[AUDIO_OUTPUT_RIGHT_OUTPUT].setVoltage(audioBufferRight[i] * 10.f);
    }
}

// Save state
json_t* BeatSlicer::dataToJson() {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "currentScale", json_string(currentScale.c_str()));
    json_object_set_new(rootJ, "currentAudioFile", json_string(currentAudioFile.c_str()));
    json_object_set_new(rootJ, "currentTuningFile", json_string(currentTuningFile.c_str()));
    return rootJ;
}

// Load state
void BeatSlicer::dataFromJson(json_t* rootJ) {
    json_t* scaleJ = json_object_get(rootJ, "currentScale");
    if (scaleJ) currentScale = json_string_value(scaleJ);

    json_t* audioFileJ = json_object_get(rootJ, "currentAudioFile");
    if (audioFileJ) currentAudioFile = json_string_value(audioFileJ);

    json_t* tuningFileJ = json_object_get(rootJ, "currentTuningFile");
    if (tuningFileJ) currentTuningFile = json_string_value(tuningFileJ);
}

BeatSlicerWidget::BeatSlicerWidget(BeatSlicer *module) {
    setModule(module);

    // Set panel
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/beatSlicer.svg")));

    // Inputs
    addInput(createInput<PJ301MPort>(Vec(30.6, 43.9), module, BeatSlicer::AUDIO_INPUT_LEFT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(62.9, 43.8), module, BeatSlicer::AUDIO_INPUT_RIGHT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(110.6, 43.5), module, BeatSlicer::GATE_INPUT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(142.9, 43.5), module, BeatSlicer::TRIGGER_INPUT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(110.3, 103.1), module, BeatSlicer::ENV_INPUT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(73.9, 121.7), module, BeatSlicer::RECORD_INPUT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(66.0, 180.0), module, BeatSlicer::SLICE_INPUT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(90.8, 243.3), module, BeatSlicer::MODE_INPUT_INPUT));
    addInput(createInput<PJ301MPort>(Vec(24.2, 316.7), module, BeatSlicer::STRETCH_INPUT_INPUT));

    // Outputs
    addOutput(createOutput<PJ301MPort>(Vec(118.6, 317.4), module, BeatSlicer::AUDIO_OUTPUT_LEFT_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(150.9, 317.6), module, BeatSlicer::AUDIO_OUTPUT_RIGHT_OUTPUT));

    // Knobs
    addParam(createParam<RoundBlackKnob>(Vec(59.2, 317.9), module, BeatSlicer::STRETCH_KNOB_PARAM));
    addParam(createParam<RoundBlackKnob>(Vec(112.6, 189.2), module, BeatSlicer::SLICE_KNOB_PARAM));
    addParam(createParam<RoundBlackKnob>(Vec(150.8, 103.1), module, BeatSlicer::ENV_KNOB_PARAM));

    // Record Button (Red Button)
    addParam(createParam<LEDButton>(Vec(33.9, 122.1), module, BeatSlicer::RECORD_BUTTON_PARAM));
    addChild(createLight<LargeLight<RedLight>>(Vec(73.9 , 102.1), module, BeatSlicer::RECORD_LIGHT_LIGHT));

    // Toggle switch
    addParam(createParam<CKSSThreeHorizontal>(Vec(28.0, 238.1), module, BeatSlicer::MODE_SWITCH_PARAM));
}
// Register the module
Model *modelBeatSlicer = createModel<BeatSlicer, BeatSlicerWidget>("beatSlicer");
