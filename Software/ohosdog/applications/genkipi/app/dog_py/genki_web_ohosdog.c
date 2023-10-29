
#include "genki_web_ohosdog.h"

#include <stdio.h>
#include "utils_file.h"
#include <stdint.h>
#include <stdlib.h>
#include "dog.h"

static void save_html(const char *page, const char *html) {
    int fd = UtilsFileOpen(page, O_CREAT_FS | O_TRUNC_FS | O_WRONLY_FS, 0);

    if (fd > 0) {
        UtilsFileWrite(fd, html, strlen(html));
        UtilsFileClose(fd);
    }
}

static int doHtml(genki_web_request_t *request, genki_web_response_t *response) {
    if (request->method != GET) {
        return -1;
    }
    const char *page = "plot.html";
    unsigned int size;
    char buf[128];
    if (UtilsFileStat(page, &size) >= 0) {
        sprintf(buf, "%d", size);
        response->setHeader(response, "Content-Length", buf);

        int fd = UtilsFileOpen(page, O_RDONLY_FS, 0);
        if (fd > 0) {
            unsigned char num;
            while ((num = UtilsFileRead(fd, buf, 128)) > 0) {
                response->write(response, buf, num);
            }
            UtilsFileClose(fd);
        } else {
            sprintf(buf, "<html><body><h2 id='a'>ERROR</h2></body></html>");
            response->write(response, buf, strlen(buf));
        }
    } else {
       const char *html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>天使战队</title>\
       <style> body {user-select: none;text-align: center;}  table {margin: 0 auto}  td div {width: 150px;height: 150px;border: 1px solid darkgrey;display: flex;align-items: center;justify-content: center;color: black;}  #s {background-color: red;color: white;}  td div:active {background: deepskyblue;color: white;}  button {margin: 0.5rem;width: 9rem;height: 3rem;font-size: 1.2rem;border-radius: 1rem;}  button:hover {border-radius: 1rem;background-color: deepskyblue;color: aliceblue;}\
       </style></head><body><h1>鸿蒙狗控制</h1>\
       <button id=\"h\">初始化</button>\
       <br><br>\
       <button id=\"b\">原地踏步</button>\
       <br><br>\
       <button id=\"d\">前进</button>\
       <br><br>\
       <button id=\"e\">后退</button>\
       <br><br>\
       <button id=\"f\">左转</button>\
       <br><br>\
       <button id=\"g\">右转</button>\
       <br><br>\
       <script> function u(p) {return `${p}`;}\
       function g(i) {return document.getElementById(i);}\
       function fe(e, f) {e.addEventListener('click', function (e) {/* do something*/f();});}\
       function fg(u, f) {let x = new XMLHttpRequest();x.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {f(x);}};x.open(\"GET\", u, true);x.send();}\
       function fp(u, d, f, p) {let x = new XMLHttpRequest();if (p) {x.upload.addEventListener('progress', function (e) {p(e);});}x.onreadystatechange = function () {if (this.readyState == 4 && this.status == 200) {f(x);}};x.open('POST', u, true);x.timeout = 45000;x.send(d);}\
       fe(g('b'), () => {let data = {\"type\": 3, \"count\": 10, \"list\": [[45, 125, 75, 100, 75, 100, 45, 125, 100], [75, 100, 45, 110, 45, 110, 75, 100, 100]]};fp(u('/dog/cmds'), JSON.stringify(data), () => {});});\
       fe(g('d'), () => {let data = {\"type\": 3, \"count\": 10, \"list\": [[32, 103, 58, 147, 67, 142, 21, 107, 80], [38, 117, 11, 139, 20, 140, 28, 120, 80], [64, 140, 7, 112, 18, 110, 55, 145, 80], [52, 147, 22, 109, 33, 105, 44, 150, 80], [2, 123, 42, 136, 52, 132, -8, 122, 80]]};fp(u('/dog/cmds'), JSON.stringify(data), () => {});});\
       fe(g('e'), () => {let data = {\"type\": 3, \"count\": 5, \"list\":  [[87, 149, 31, 124, 41, 120, 77, 156, 80], [69, 143, 18, 139, 27, 139, 60, 148, 80], [42, 122, 67, 161, 74, 155, 32, 126, 80], [32, 126, 74, 155, 84, 148, 22, 128, 80], [47, 155, 42, 135, 51, 131, 40, 157, 80]]};fp(u('/dog/cmds'), JSON.stringify(data), () => {});});\
       fe(g('f'), () => {let data = {\"type\": 3, \"count\": 10, \"list\": [[63, 139, 19, 100, 63, 139, 19, 100, 80], [48, 128, 16, 117, 39, 132, 25, 115, 80], [31, 98, 48, 144, 27, 100, 51, 142, 80], [57, 140, 20, 103, 31, 98, 48, 144, 80], [26, 115, 38, 132, 48, 128, 16, 117, 80]]};fp(u('/dog/cmds'), JSON.stringify(data), () => {});});\
       fe(g('g'), () => {let data = {\"type\": 3, \"count\": 10, \"list\": [[31, 96, 54, 144, 31, 96, 54, 144, 80], [35, 112, 22, 136, 16, 117, 38, 132, 80], [60, 138, 13, 105, 54, 141, 20, 103, 80], [23, 102, 51, 142, 60, 138, 13, 105, 80], [31, 134, 25, 115, 35, 112, 22, 136, 80]]};fp(u('/dog/cmds'), JSON.stringify(data), () => {});});\
       fe(g('h'), () => {let data = {\"type\": 3, \"count\": 1, \"list\":  [[45, 135, 45, 135, 45, 135, 45, 135, 300]]};fp(u('/dog/cmds'), JSON.stringify(data), () => {});});\
       </script></body></html>";

        size_t len = strlen(html);

        //char buf[128];
        char buf[1024];
        sprintf(buf, "%d", len);
        response->setHeader(response, "Content-Type", "text/html; charset=UTF-8");
        response->setHeader(response, "Content-Length", buf);
        response->write(response, html, len);
        save_html(page, html);
    }

    return 1;
}

static int doInit(genki_web_request_t *request, genki_web_response_t *response) {
    dog_install();

    return 1;
}

static int doCmds(genki_web_request_t *request, genki_web_response_t *response) {
    if (request->method == GET) return -1;

    int contentLength = 0;
    char *cl = request->getHeader(request, "Content-Length");
    if (cl == NULL) {
        return -1;
    }
    contentLength = atoi(cl);
    char buf[2048];
    int num;
    num = request->read(request, buf, 2048);

    dog_execJson(buf, num);

    return 1;
}


genki_web_service_t *newDogService(void) {
    genki_web_service_t *service = genki_web_newService("DOG");
    service->addFilter(service, "/dog", doHtml);
    service->addFilter(service, "/dog/init", doInit);
    service->addFilter(service, "/dog/cmds", doCmds);

    service->link_name = "Dog控制";
    service->link_url = "/dog";
    return service;
}