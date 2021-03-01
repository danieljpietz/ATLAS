//
//  main.m
//  LegBackendiOS
//
//  Created by Daniel Pietz on 2/1/21.
//

#import <UIKit/UIKit.h>
#include <iostream>
#import "AppDelegate.h"
#include "Header.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    main2();
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
