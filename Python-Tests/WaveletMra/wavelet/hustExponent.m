#!/usr/bin/octave-cli -qf
% hurstExponent.m
%
% Licensing:
%
%       This code is distributed under the GNU LGPL license.
%
% Modified:
%
%       26 March 2017
%
% Author:
%
%       Anderson Paschoalon
%
% Command line args:
%
%	arg1: experiment name1
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Initialization and load data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%inicialization
arg_list = argv ();
close all; clc; clf;
graphics_toolkit ("gnuplot");
set(0, 'DefaultLineLineWidth', 4);
set(0,'defaulttextfontsize', 14);
set(0,'DefaultAxesFontSize',12)
set(0,'DefaultAxesFontName', 'Times New Roman')
EXPERIMENT_NAME1 = arg_list{1};
EXPERIMENT_NAME2 = arg_list{2};

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Constants
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% plot directory whre the gnuplot data files will be saved
PLOT_DIR = '../../plots/';

% gnuplot data extension
PLOT_DATA_EXT = ".dat";

% path and datafile prefix. file generated by pcap-sniffer.py
DATAFILE_NAME_PREFIX = "../data/packetsinfo_";

%  file extension
DATAFILE_NAME_EXT = ".txt";

% path+file with packet data to be loaded
SIMULATION_DATA1 = strcat(DATAFILE_NAME_PREFIX, EXPERIMENT_NAME1, DATAFILE_NAME_EXT);
SIMULATION_DATA2 = strcat(DATAFILE_NAME_PREFIX, EXPERIMENT_NAME2, DATAFILE_NAME_EXT);

% load basic data 
TraceData1 = dlmread(SIMULATION_DATA1, ' ' , 2, 0);
TraceData2 = dlmread(SIMULATION_DATA2, ' ' , 2, 0);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Wavelet multiresolution energy analizis (MREA)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
interArrival1 = TraceData2(:, 4);
h1 = hurst(interArrival1);
interArrival2 = TraceData2(:, 4);
h2 = hurst(interArrival2);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Save data 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
h = [h1 h2];
title = "Hust Exponent";
labels = strcat('Hurst:',EXPERIMENT_NAME1 ,', Hurst:',EXPERIMENT_NAME2);
filename = strcat(PLOT_DIR, title, PLOT_DATA_EXT);
matrix2File(h, filename, title, labels);





















