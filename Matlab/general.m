clear all, close all;

addpath('irsensor');

irsensor_calibrate('test_data.dat', 'test_calibrated_values', 'test_image', false);
