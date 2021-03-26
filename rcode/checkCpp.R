library(ggplot2)
library(tidyr)

df <- read.csv("/home/thinh/Downloads/cpp_training-main/output/HN.csv")

df_plot <- gather(df, key = "Compartment", value = "Value", -Time)
df_plot$Compartment <- factor(df_plot$Compartment)
df_plot$Time <- as.numeric(df_plot$Time)
str(df_plot)

ggplot(df_plot, aes(x = Time, y = Value, color = Compartment)) + geom_line()
df$sum <- rowSums(df[,grep("Time", colnames(df), invert = T)])
table(df$sum)
# ggsave("/home/thinh/Dropbox/oucru/cpp/cpp_training/plots/test_r0_3_diffWaitingTime.png", width = 7, height = 3)
