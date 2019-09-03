# Module 2 - Set up Azure IoT Edge

In this module, we are going to do the following:
- set up a virtual machine in Azure for our IoT Edge device
- create the IoT Edge device in Azure and configure Edge on the VM
- configure that IoT Edge device as a transparent gateway
- download and store the root CA certificate needed for our MXChip

## Setup IoT Edge virtual machine

In this section, we are going to create and setup an Azure VM as the IoT Edge box.  Technically, it a 'real' deployment, you would use something on-premises, such as a local hardware device (PC, Raspberry Pi, VM, etc) as your IoT Edge device.  However, just for ease of setup here, we are going to use a VM in Azure. If you already have, or prefer to use, a local device, feel free. For that option, you can find the IoT Edge setup instructions here (TODO: link)

To create the IoT Edge VM in Azure, we are going to creat the VM via the Azure CLI. Specifically, to make things even easier, we are going to use a "cloud shell" in Azure to keep from having to install the Azure CLI locally.

To create a "cloud shell" instance, login to the Azure portal, and click the button that looks like a ">_" next to the search bar at the top

![cloud shell icon](/images/cloud-shell-icon.png)

You may be required to provision some storage to create the cloud shell. For this lab, choose "bash" as the type of shell.

Once you have the shell launched, we are ready to create a Virtual Machine. For this VM, we are going to use the pre-built "Ubuntu 16.04 with Azure IoT Edge runtime" VM.  This is a version of Ubuntu 16.04 Linux with the IoT Edge runtime pre-installed.

Before we create the VM, let's create a resource group in Azure to contain our project services, if you don't already have one (if you already have one, feel free to skip)

In the cloud shell, run the following command to create your resource group

```bash
az group create --name {your resource group name} --location {location}
```

where {your resource group name} is the name you want to call your resource group and {location} is the Azure region in which you want to work (i.e. 'centralus'). You can find a list of available azure regions (and their abbreviations) by running this command 'az account list-locations -o table' in the azure CLI

To create the VM, run this command

```bash
az vm create \
--resource-group [resource_group] \
--name [vm name] \
--location [location] \
--image microsoft_iot_edge:iot_edge_vm_ubuntu:ubuntu_1604_edgeruntimeonly:1.0.1 \
--admin-username [userid] \
--admin-password [password] \
--authentication-type password \
--size Standard_B1ms \
--public-ip-address-dns-name [vm name]
```

where
- resource_group is the name you used above when created the resource group
- vm name is a globally unique name for your VM
- location is the location you created your RG in above
- userid is the login id you want to use for your VM
- password is a very strong password you want to use for the userid above

This will create a VM with the iot edge runtime already installed and assign it the DNS name [vm name].[location].cloudapp.azure.com   (for example steveiotedgevm.centralus.cloudapp.azure.com).  

NOTE:  note the DNS name of your VM, as you will use that later in the IoT Edge setup and to point your MXChip at it

# Creat IoT Edge device in Azure and configure IoT Edge on the VM

Before we do anything related to IoTHub, we need to add the CLI extension for IoT. To do this, run the following command:

```bash
az extension add --name azure-cli-iot-ext
```
You should only need to do that once.

If you haven't already created an IoT Hub, you can do so with this command in the CLI

```bash
az iot hub create --name {your iot hub name} --resource-group {your resource group name} --sku S1
```

{your iot hub name} needs to be a globally unique name for your hub

Once the IoT Hub is created, you can create an IoT Edge device in your IoT Hub with this command

```bash
az iot hub device-identity create --device-id [device id] --hub-name [hub name] --edge-enabled
```

where [device id] is a name you make up for your edge device.  It does not need to be globally unique, just unique within your hub.  No underscores, no spaces!

Once the device is created, we need to grab it's connection string

```bash
az iot hub device-identity show-connection-string --device-id [device id] --hub-name [hub name]
```

Copy the connection string returned over to notepad or similar, we will need it later






