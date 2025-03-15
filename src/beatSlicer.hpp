#pragma once
#include "plugin.hpp"

struct BeatSlicer : Module {
    enum ParamIds {
        STRETCH_KNOB_PARAM,
        SLICE_KNOB_PARAM,
        RECORD_BUTTON_PARAM,
        ENV_KNOB_PARAM,
        MODE_SWITCH_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        AUDIO_INPUT_LEFT_INPUT,
        AUDIO_INPUT_RIGHT_INPUT,
        GATE_INPUT_INPUT,
        TRIGGER_INPUT_INPUT,
        ENV_INPUT_INPUT,
        RECORD_INPUT_INPUT,
        SLICE_INPUT_INPUT,
        MODE_INPUT_INPUT,
        STRETCH_INPUT_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        AUDIO_OUTPUT_LEFT_OUTPUT,
        AUDIO_OUTPUT_RIGHT_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        RECORD_LIGHT_LIGHT,
        NUM_LIGHTS
    };

    // Constructor declaration
    BeatSlicer();

    // Audio buffers
    std::vector<float> audioBufferLeft;
    std::vector<float> audioBufferRight;
    bool isRecording = false;

    // Slice management
    std::vector<size_t> slicePoints;
    size_t currentSlice = 0;

    // Pitch detection and microtonal tuning
    std::vector<float> slicePitches;
    std::string currentScale = "12TET";
    std::string currentAudioFile = "";
    std::string currentTuningFile = "";

    // Pitch detection algorithm (placeholder)
    float detectPitch(const std::vector<float>& buffer, size_t start, size_t end);

    // Microtonal tuning
    float mapPitchToScale(float pitch, const std::string& scale);

    // Slice creation
    void autoDetectSlices();

    // Process function
    void process(const ProcessArgs &args) override;

    // Save and load state
    json_t* dataToJson() override;
    void dataFromJson(json_t* rootJ) override;
};

struct BeatSlicerWidget : ModuleWidget {
    BeatSlicerWidget(BeatSlicer *module);
};
