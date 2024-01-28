#!/usr/bin/env python

from itertools import product
import os
from jinja2 import Environment, FileSystemLoader
from pathlib import Path
import shutil
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import shutil
import subprocess
from datetime import datetime

# ------------ change variables in this block before running -------
# jpscore = "/Users/chraibi/workspace/jupedsim/jpscore/build/bin/jpscore"
jpscore = "C:/Program Files/JuPedSim 0.9.6/bin/jpscore.exe"

PATH = os.path.abspath('')
TEMPLATE_ENVIRONMENT = Environment(
    autoescape=False,
    loader=FileSystemLoader(PATH),
    trim_blocks=False)


def render_template(template_filename, context):
    return TEMPLATE_ENVIRONMENT.get_template(template_filename).render(context)


def Product(variables):
    return list(product(*variables))

# fname holds the full path
def create_inifile(fname, results_file_path, operational_model_id, events_file_name):
    context = {
               'results_file_path': results_file_path,
               'operational_model_id': operational_model_id,
               'events_file_name': events_file_name,
               }
    with open(fname, 'w') as f:
        xml = render_template(master_file, context)
        f.write(xml)


if __name__ == "__main__":
    operational_model_id_dict = {
                                    3: "Tordeux2015",
                                    1: "Gcfm"
                                }
    events_file_name_list = [
        "events_a.xml",
        "events_b.xml",
        "events_c.xml",
    ]
    
    master_file_list = ["master_300.xml", "master_500.xml"]
    # To test the system
    master_file_list = ["master_50.xml"]
    
    
    curr_dir = os.getcwd().replace("\\", "/") + "/"
    
    output_txt_list = []
    
    # generate the folder and place the related file 
    for operational_model in operational_model_id_dict.keys():
        for event_file in events_file_name_list:
            for master_file in master_file_list:
                folder_name = f"{master_file}-{event_file}-{operational_model_id_dict[operational_model]}/"
                folder_name = folder_name.replace("master_", "").replace(".xml", "")
                result_f_name = f"res_{folder_name}"
                
                # create the folder
                os.makedirs(folder_name, exist_ok=True)
                infilename = curr_dir + folder_name + folder_name.replace("/", ".xml")
                
                create_inifile(infilename, result_f_name, operational_model, event_file)
                
                # warning, change dir 
                os.chdir(curr_dir+folder_name)
                
                # copying geometry and event file
                shutil.copyfile(curr_dir+"geometry.xml", "geometry.xml")
                shutil.copyfile(curr_dir+event_file, event_file)
                
                # run from the subprocess
                output_txt = subprocess.run([jpscore, infilename], capture_output=True)
                output_txt_list.append(output_txt)
                # print(output_txt)
                
                # TODO: Threading of Multiprocessing
                
                # back to main dir
                os.chdir(curr_dir)
                
    
    with open(f"output_{str(datetime.now()).replace(':', '-')}.txt", "w+") as f:
        f.write(
            str(output_txt_list)
        )
                
                
                

    
    
