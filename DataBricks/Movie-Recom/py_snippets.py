# ======================== #
# Mounting data #
# ======================== #

storage_account_name = "movierecom1"
container_name = "validated"
root_dir ="/mnt/Files/validated"

application_id = dbutils.secrets.get(scope="movieScope", key="clientid")
authentation_key = dbutils.secrets.get(scope="movieScope", key="clientsecret")
tenant_id = dbutils.secrets.get(scope="movieScope", key="tenantid")

endpoint = "https://login.microsoftonline.com/" + tenant_id + "/oauth2/token"
source = "abfss://"+container_name+"@"+storage_account_name+".dfs.core.windows.net"

configs = {"fs.azure.account.auth.type": "OAuth",
          "fs.azure.account.oauth.provider.type": "org.apache.hadoop.fs.azurebfs.oauth2.ClientCredsTokenProvider",
          "fs.azure.account.oauth2.client.id": application_id,
          "fs.azure.account.oauth2.client.secret": authentation_key,
          "fs.azure.account.oauth2.client.endpoint": endpoint}

# Optionally, you can add <directory-name> to the source URI of your mount point.
# dbutils.fs.mount(
#   source = "abfss://<container-name>@<storage-account-name>.dfs.core.windows.net/",
#   mount_point = "/mnt/<mount-name>",
#   extra_configs = configs)

if not any(mount.mountPoint==mountPoint for mount in dbutils.fs.mounts()):
    dbutils.fs.mount(
    source = source,
    mount_point = mountPoint,
    extra_configs = configs)