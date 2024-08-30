#!/usr/bin/python3

import numpy as np
import pandas as pd
from datetime import datetime
import argparse

import uproot


def dt5202_text(file_name, mapping = None):
        with open(file_name, 'r') as file:
                tstamps = []
                trgids = []

                boards = []
                channels = []
                LGs = []
                HGs = []

                boards_evt = []
                channels_evt = []
                LGs_evt = []
                HGs_evt = []

                layers = []
                assemblies = []
                layer_chs = []
                layer_rows = []
                layer_cols = []  

                layer_evt = []
                assembly_evt = []
                layer_ch_evt = []
                layer_row_evt = []
                layer_col_evt = []


                for line in file:
                        
                        if line.startswith('//'):
                                pass
                        elif 'Tstamp_us' in line:
                                pass
                        else:
                                tokens = line.split()
                                if len(tokens) == 6:
                                        if len(tstamps) > 0:
                                                boards.append(boards_evt)
                                                channels.append(channels_evt)
                                                LGs.append(LGs_evt)
                                                HGs.append(HGs_evt)
                                                layers.append(layer_evt)
                                                assemblies.append(assembly_evt)
                                                layer_chs.append(layer_ch_evt)
                                                layer_rows.append(layer_row_evt)
                                                layer_cols.append(layer_col_evt)
                                                
                                                boards_evt = []
                                                channels_evt = []
                                                LGs_evt = []
                                                HGs_evt = []
                                                layer_evt = []
                                                assembly_evt = []
                                                layer_ch_evt = []
                                                layer_row_evt = []
                                                layer_col_evt = []


                                        tstamps.append(float(tokens[0]))
                                        trgids.append(int(tokens[1]))
                                        boards_evt.append(int(tokens[2]))
                                        channels_evt.append(int(tokens[3]))
                                        LGs_evt.append(int(tokens[4]))
                                        HGs_evt.append(int(tokens[5]))

                                        if mapping:
                                            try:
                                                layer, assembly, layer_ch, layer_row, layer_col = mapping[(int(tokens[2]), int(tokens[3]))]
                                                layer_evt.append(layer)
                                                assembly_evt.append(assembly)
                                                layer_ch_evt.append(layer_ch)
                                                layer_row_evt.append(layer_row)
                                                layer_col_evt.append(layer_col)
                                            except KeyError:
                                                print(f'No mapping for board {int(tokens[2])} channel {int(tokens[3])}')
                                                layer_evt.append(-1)
                                                assembly_evt.append(-1)
                                                layer_ch_evt.append(-1)
                                                layer_row_evt.append(-1)
                                                layer_col_evt.append(-1)
                                        else:
                                            layer_evt.append(-1)
                                            assembly_evt.append(-1)
                                            layer_ch_evt.append(-1)
                                            layer_row_evt.append(-1)
                                            layer_col_evt.append(-1)

                                elif len(tokens) == 4:
                                        boards_evt.append(int(tokens[0]))
                                        channels_evt.append(int(tokens[1]))
                                        LGs_evt.append(int(tokens[2]))
                                        HGs_evt.append(int(tokens[3]))

                                        if mapping:
                                            try:
                                                layer, assembly, layer_ch, layer_row, layer_col = mapping[(int(tokens[0]), int(tokens[1]))]
                                                layer_evt.append(layer)
                                                assembly_evt.append(assembly)
                                                layer_ch_evt.append(layer_ch)
                                                layer_row_evt.append(layer_row)
                                                layer_col_evt.append(layer_col)
                                            except KeyError:
                                                print(f'No mapping for board {int(tokens[0])} channel {int(tokens[1])}')
                                                layer_evt.append(-1)
                                                assembly_evt.append(-1)
                                                layer_ch_evt.append(-1)
                                                layer_row_evt.append(-1)
                                                layer_col_evt.append(-1)
                                        else:
                                            layer_evt.append(-1)
                                            assembly_evt.append(-1)
                                            layer_ch_evt.append(-1)
                                            layer_row_evt.append(-1)
                                            layer_col_evt.append(-1)



                boards.append(boards_evt)
                channels.append(channels_evt)
                LGs.append(LGs_evt)
                HGs.append(HGs_evt)
                layers.append(layer_evt)
                assemblies.append(assembly_evt)
                layer_chs.append(layer_ch_evt)
                layer_rows.append(layer_row_evt)
                layer_cols.append(layer_col_evt)

                                
        return  tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols

       

def flatten(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols):
        tstamps_flat = []
        trgids_flat = []
        
        boards_flat = []
        channels_flat = []
        LGs_flat = []
        HGs_flat = []

        layers_flat = []
        assemblies_flat = []
        layer_chs_flat = []
        layer_rows_flat = []
        layer_cols_flat = []

        for i in range(len(tstamps)):
                for j in range(len(channels[i])):
                        tstamps_flat.append(tstamps[i])
                        trgids_flat.append(trgids[i])

                        boards_flat.append(boards[i][j])
                        channels_flat.append(channels[i][j])
                        LGs_flat.append(LGs[i][j])
                        HGs_flat.append(HGs[i][j])

                        layers_flat.append(layers[i][j])
                        assemblies_flat.append(assemblies[i][j])
                        layer_chs_flat.append(layer_chs[i][j])
                        layer_rows_flat.append(layer_rows[i][j])
                        layer_cols_flat.append(layer_cols[i][j])

        return np.array(tstamps_flat), np.array(trgids_flat), np.array(boards_flat), np.array(channels_flat), np.array(LGs_flat), np.array(HGs_flat), np.array(layers_flat), np.array(assemblies_flat), np.array(layer_chs_flat), np.array(layer_rows_flat), np.array(layer_cols_flat)


# def flatten(tstamps, trgids, boards, channels, LGs, HGs):
#         tstamps_flat = []
#         trgids_flat = []
        
#         boards_flat = []
#         channels_flat = []
#         LGs_flat = []
#         HGs_flat = []

#         for i in range(len(tstamps)):
#                 for j in range(len(channels[i])):
#                         tstamps_flat.append(tstamps[i])
#                         trgids_flat.append(trgids[i])

#                         boards_flat.append(boards[i][j])
#                         channels_flat.append(channels[i][j])
#                         LGs_flat.append(LGs[i][j])
#                         HGs_flat.append(HGs[i][j])

#         return np.array(tstamps_flat), np.array(trgids_flat), np.array(boards_flat), np.array(channels_flat), np.array(LGs_flat), np.array(HGs_flat)

def save_as_numpy(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols, file_out):
        tstamps_flat, trgids_flat, boards_flat, channels_flat, LGs_flat, HGs_flat, layers_flat, assemblies_flat, layer_chs_flat, layer_rows_flat, layer_cols_flat = flatten(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols)

        data = np.array([tstamps_flat, trgids_flat, boards_flat, channels_flat, LGs_flat, HGs_flat,layers_flat, assemblies_flat, layer_chs_flat, layer_rows_flat, layer_cols_flat])
        np.save(file_out, data)


def save_as_root(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols, file_out):
    file = uproot.recreate(file_out)

    file["tree"] = {"t_stamp": tstamps, 
                    "trgid": trgids,
                    "board": boards,
                    "channel": channels,
                    "LG": LGs,
                    "HG": HGs,
                    "layer": layers,
                    "assembly": assemblies,
                    "layer_ch": layer_chs,
                    "layer_row": layer_rows,
                    "layer_col": layer_cols}
    file.close()

def save_as_dataframe(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols, file_out):
        tstamps_flat, trgids_flat, boards_flat, channels_flat, LGs_flat, HGs_flat, layers_flat, assemblies_flat, layer_chs_flat, layer_rows_flat, layer_cols_flat = flatten(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols)

        df = pd.DataFrame({'t_stamp': tstamps_flat,
                   'trgid': trgids_flat,
                   'board': boards_flat,
                   'channel': channels_flat,
                   'LG': LGs_flat,
                   'HG': HGs_flat,
                   'layer': layers_flat,
                   'assembly': assemblies_flat,
                   'layer_ch': layer_chs_flat,
                   'layer_row': layer_rows_flat,
                   'layer_col': layer_cols_flat})

        df.to_pickle(file_out)

def is_valid_mapping(parser, mapping):
    if not mapping:
        return False
    try:
        with open(mapping, 'r') as file:
            mapping = {}
            for line in file:
                if line.startswith('#'):
                    continue
                if len(line.strip()) == 0:
                    continue
                tokens = line.split()
                if len(tokens) != 7:
                    print(line)
                    parser.error('Mapping file must have 7 columns')
                for t in tokens:
                    if not t.isdigit():
                        parser.error('All columns in mapping file must be integers')
                caen_board = int(tokens[0])
                caen_ch = int(tokens[1])
                layer = int(tokens[2])
                assembly = int(tokens[3])
                layer_ch = int(tokens[4])
                layer_row = int(tokens[5])
                layer_col = int(tokens[6])

                mapping[(caen_board, caen_ch)] = (layer, assembly, layer_ch, layer_row, layer_col)

        return mapping
    except FileNotFoundError:
        parser.error('Mapping file not found')
    except Exception as e:
        parser.error(f'Error reading mapping file: {e}')
    return False

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--mapping', type = lambda x:is_valid_mapping(parser, x),help='mapping file')
    parser.add_argument('-r', '--root', default=False, action='store_true', help='output as root file')
    parser.add_argument('-p', '--pandas', default=False, action='store_true', help='output as pickled pandas dataframe')
    parser.add_argument('-n', '--numpy',  default=False, action='store_true', help='output as numpy file')
    parser.add_argument('file', type=str, nargs='+', help='Input file(s)')
    
    args = parser.parse_args()

    if args.mapping:
        print(f'Using mapping file: {args.mapping}')
    else:
        print('No mapping file specified')


    if not (args.root or args.pandas or args.numpy):
        parser.error('No output format specified, add -r, -p or -n')
    
    if not args.file:
        parser.error('No input file specified')

    for f in args.file:
        if not f.endswith('.txt'):
            parser.error('Input file must be a .txt file')
        
    for f in args.file:
        print(f'{f}: ', end = '')
        tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols = dt5202_text(f, args.mapping)
        if args.root:
            file_out_root = f.replace('.txt', '.root')
            save_as_root(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols, file_out_root)
            print(f'root ', end = '')    
        if args.pandas:
            file_out_pkl = f.replace('.txt', '.pkl')
            save_as_dataframe(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols, file_out_pkl)
            print(f'pandas ', end = '')    
        if args.numpy:
            file_out_npy = f.replace('.txt', '.npy')
            save_as_numpy(tstamps, trgids, boards, channels, LGs, HGs, layers, assemblies, layer_chs, layer_rows, layer_cols, file_out_npy)
            print(f'numpy ', end = '')    

    print('done.')


