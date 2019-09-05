# Module 4 - Edge Processing with Azure Streaming Analytics

In this module, we are going to take the messages from our "dumb" MXChip device, which blindly sends data once per second, and do some 'edge processing' with Azure Stream Analytics (ASA) to aggregate and slow down that data rate at the Edge.  This is not an advanced scenario for Edge, but rather an easy straightforward way to show the value of connecting your on-premises IoT devices through IoT Edge

## Set up a storage account

In order to deploy our ASA job to Edge, the first step we need to do is to create a storage account in Azure.  The storage account is used to hold the details of the job and as a place from which the ASA on Edge module can download it.

To create a storage account:

* in the Azure portal, navigate to "Storage accounts" on the left-hand nav and click "Add"
  * give the account a name (which has to be globally unique)
  * for resource group, use the same one we used for the ASA job and IoT Hub
  * leave all the other settings at default and click "Create"
  * after the account gets created, navigate to it and click "Browse Blobs"
  * Create a new container (give it a name) and for access, choose "Container" level and select "Ok"

## Creating the ASA job

Our ASA job will be set up to take the messages produced by our formatter module in module 3 of the labs.

An ASA job consists of three parts:

* one or more "inputs", or sources of either streaming or reference data
* one or more "outputs" or sinks for the query results
* the "query" that represents the processing of the job, which takes data from the inputs, processes it, and writes the results to one or more outputs

To create the ASA job:

* in the Azure portal, navigate in the left nav bar to the Azure Stream Analytics jobs blade and click Add
  * give your ASA job a unique name (e.g. edgeASAJob)
  * under resource group, choose "use existing" and pick the same resource group that contains your IoT Hub
  * for Hosting Environment, choose "Edge" to indicate that we plan to host this ASA job on IoT Edge.  The same ASA job could, alternately, be hosted in the cloud and aggregate the data *after* it made it to IoT Hub.
  * click "Create"

After a few moments, your ASA job will be created.  Click "Go to Resource" to navigate to the portal blade for your job

* click on the "inputs" box and click "Add stream input", and click "Edge Hub"
* give your input the name "inputFromHub" and leave the other settings as default.
* click Create to create your input

Navigate back to the blade for your ASA job and click on the "outputs" box to add outputs

* click Add to create a new output and choose "Edge Hub"
* name your output 'outputToHub' for the aggregated temperature data
* leave the remaining settings at default and click "Create"

The next step is to specify the query.  In the upper right of the query window, click on the "Edit query" to open the query editor.  Copy/paste in this query:


TODO:  get device id from 'connected device'
```SQL
SELECT
    deviceID,
    AVG(temperature) as AvgTemp,
    AVG(humidity) as AvgHumidity,
    System.Timestamp as eventdatetime
INTO outputToHub
FROM inputFromHub
WHERE temperature IS NOT NULL
GROUP BY
    deviceID,
    TumblingWindow(s, 30)
```    

* Click Save to save your query and close the editor

* On the ASA job blade and click on the "IoT Edge Settings" tab in the left nav
* Select your storage account and container that you just created and click Save.

## Deploy our ASA job

Now we are ready to deploy our ASA job to our Edge device.

* navigate back to your IoT Hub and then to your Edge device
* click "Set Modules"
* select "Add" and then "Azure Stream Analytics Module"
* select your subscription, and the ASA job you created earlier.  After a few moments you'll get a notification that the ASA job has been successfully published
* click "next" to configure our routes
* on the 'specify routes' screen, enter the following routes

```json
{
  "routes": {
    "toASAfromDevices": "FROM /messages/* WHERE NOT IS_DEFINED($connectionModuleId) INTO BrokeredEndpoint(\"/modules/edgeASAJob/inputs/inputFromHub\")",
    "asaAggrToIoTHub": "FROM /messages/modules/edgeASAJob/outputs/* INTO $upstream"
  }
}
```

* replace "edgeASAJob" above with the name of your ASA module (if it was different than edgeASAJob).  Also, replace 'formattermodule' in the route JSON if you used a different name in module 3.

click 'next' and 'finish' to publish our routes.

After a few minutes, on your IoT Edge box, you'll see the ASA module get deployed. You can see the list of running modules by running

```bash
sudo iotedge list
```

## Seeing results

If you look at the monitor-events output in the Azure portal, you should no longer see the 'every second' output, but rather you now see the aggregated output every 30 seconds.  So we are processing and aggregating data the edge.

Now that you have your IoT device data flowing through IoT Edge, you can do all the many more sophisticated edge processing use cases.

Enjoy.
