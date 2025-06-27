#include <QCoreApplication>
#include <QException>
#include <QDebug>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT     "8554"
#define USER_ID         "admin"
#define USER_PASSCODE   "password"
#define MOUNT_POINTS    "/webcam"

void m_GetIPaddress()
{
    struct ifaddrs *interfaces = nullptr;
    struct ifaddrs *ifa = nullptr;

    if (getifaddrs(&interfaces) == -1) {
        perror("getifaddrs");
        return ;
    }

    for (ifa = interfaces; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) continue;

        // Skip if interface name contains "docker"
        std::string ifname(ifa->ifa_name);
        if (ifname.find("docker") != std::string::npos) continue;

        int family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) { // IPv4
            char ip[INET_ADDRSTRLEN];
            void* addr_ptr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, addr_ptr, ip, sizeof(ip));
            g_print("    %s\n",ip);

        }
        /*else if (family == AF_INET6) { // IPv6 (optional)
            char ip6[INET6_ADDRSTRLEN];
            void* addr_ptr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            inet_ntop(AF_INET6, addr_ptr, ip6, sizeof(ip6));
            std::cout << "Interface: " << ifa->ifa_name << " | IPv6: " << ip6 << std::endl;
        }*/
    }

    freeifaddrs(interfaces);
}

int main(int argc, char *argv[]) {
    try {
        gst_init(&argc, &argv);

        GstRTSPServer *server = gst_rtsp_server_new();
        gst_rtsp_server_set_service(server, SERVER_PORT);

        // ---------- Authentication Setup ----------
        GstRTSPAuth *auth = gst_rtsp_auth_new();
        gchar *basic = gst_rtsp_auth_make_basic(USER_ID, USER_PASSCODE);

        // Add token with "user" role
        GstRTSPToken *token = gst_rtsp_token_new(GST_RTSP_TOKEN_MEDIA_FACTORY_ROLE, G_TYPE_STRING, "user", NULL);
        gst_rtsp_auth_add_basic(auth, basic, token);
        gst_rtsp_server_set_auth(server, auth);

        g_free(basic);
        gst_rtsp_token_unref(token);

        // ---------- Mount & Factory ----------
        GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);
        GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

        // with Video only
        //gst_rtsp_media_factory_set_launch(factory,
        //    "( v4l2src device=/dev/video0 ! videoconvert ! video/x-raw,format=I420 ! x264enc tune=zerolatency bitrate=512 speed-preset=ultrafast ! rtph264pay config-interval=1 name=pay0 pt=96 )");

        // with Audio & Video
        gst_rtsp_media_factory_set_launch(factory,
                                          "( "
                                          "v4l2src device=/dev/video0 ! videoconvert ! video/x-raw,format=I420 ! "
                                          "x264enc tune=zerolatency bitrate=512 speed-preset=ultrafast ! rtph264pay config-interval=1 name=pay0 pt=96 "
                                          " alsasrc device=hw:0 ! audioconvert ! audioresample ! "
                                          " opusenc bitrate=64000 ! rtpopuspay name=pay1 pt=97 "
                                          ")");

        gst_rtsp_media_factory_set_shared(factory, TRUE);

        // Add role-based permissions for "user"
        GstRTSPPermissions *permissions = gst_rtsp_permissions_new();
        gst_rtsp_permissions_add_role(permissions, "user",
                                      GST_RTSP_PERM_MEDIA_FACTORY_ACCESS, G_TYPE_BOOLEAN, TRUE,
                                      GST_RTSP_PERM_MEDIA_FACTORY_CONSTRUCT, G_TYPE_BOOLEAN, TRUE,
                                      NULL);
        gst_rtsp_media_factory_set_permissions(factory, permissions);

        gst_rtsp_mount_points_add_factory(mounts, MOUNT_POINTS, factory);
        g_object_unref(mounts);

        // ---------- Run Server ----------
        guint res = gst_rtsp_server_attach(server, NULL);
        g_print("RTSP stream with auth ready at rtsp://admin:password@<IP>:8554/webcam %d \n",res);
        g_print("RTSP server IPs :\n");
        m_GetIPaddress();

        //g_print("Press Ctrl+C to close/terminate");

        GMainLoop *loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(loop);

        // Receiver
        // gst-launch-1.0 rtspsrc location=rtsp://admin:password@<IP>:8554/webcam latency=0 ! decodebin ! autovideosink
    }
    catch(QException &exp)
    {
        qDebug() << "Exception found !" << exp.what() ;
    } catch (...) {
        qDebug() << "Caught unknown exception.";
    }

    return 0;
}
